

#include <array>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#define LENGTH 8000.0
#define HEIGHT 4571.0
#define ITERATIONS 1000.0

int main() {
    unsigned int seed =
        std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(1, 3);
    int choice = distribution(generator);
    choice = 2;
    std::array<unsigned char, 14> bmp = {
        0x42, 0x4D, 0x76, 0xF3, 0x89, 0x06, 0x00,
        0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00,
    };
    std::array<unsigned char, 40> dib = {
        0x28, 0x00, 0x00, 0x00,  // number of bytes in this array
        0x40, 0x1F, 0x00, 0x00,  // Length in pixels
        0xDB, 0x11, 0x00, 0x00,  // Height in pixels // 9th and 10th
        0x01, 0x00,              // number of color planes
        0x18, 0x00,              // 0x18 0x00 number of bits per pixel
        0x00, 0x00, 0x00, 0x00,  // compression
        0x40, 0xF3, 0x89, 0x06,  // size of image data in bytes
        0x13, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    std::vector<unsigned char> image_data;
    int count = 0;
    unsigned char blue, green, red;
    double x0, y0, x, y, xtemp;
    int iter = 0, tempint;
    for (int i = 0; i < HEIGHT; i++) {
        if (i % 50 == 0) {
            std::cerr << "\rLines remaining: " << HEIGHT - i << " out of "
                      << HEIGHT << " " << std::flush;
        }
        for (int j = 0; j < LENGTH; j++) {
            x0 = ((double)j * (1.0 - -2.5)) / LENGTH + -2.5;
            y0 = ((double)i * (1.0 - -1.0)) / HEIGHT + -1.0;
            x = 0.0;
            y = 0.0;
            iter = 0;
            while (x * x + y * y <= 4 && iter < ITERATIONS) {
                xtemp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtemp;
                iter += 1;
            }
            count += iter;
            switch (choice) {
                case 1:
                    blue = 255 - ((double)iter / ITERATIONS) * 255.0;
                    green = blue;
                    red = blue;
                    break;
                case 2:
                    green = ((((double)iter / ITERATIONS)) * 255.0) * 3;
                    tempint = green * 4;
                    blue = tempint > 255 ? 255 : tempint;
                    red = std::sin(blue / 3) * 255;
                    break;
                case 3:
                    blue = ((double)iter / ITERATIONS) * 255.0;
                    green = blue;
                    red = blue;
                    break;
            }
            image_data.push_back(blue);
            image_data.push_back((unsigned char)green);
            image_data.push_back((unsigned char)red);
        }
    }
    std::cerr << "\rLines remaining: " << 0 << " out of " << LENGTH << " "
              << std::flush << std::endl;

    // Average number of iterations per pixel
    std::cout << ((double)count) / ((double)(LENGTH * HEIGHT)) << std::endl;

    std::ofstream ofs("mandelbrot_" + std::to_string(choice) + ".bmp",
                      std::ios::out | std::ios::binary);
    if (ofs.is_open()) {
        for (auto i = bmp.begin(); i != bmp.end(); i++) {
            ofs << *i;
        }
        for (auto i = dib.begin(); i != dib.end(); i++) {
            ofs << *i;
        }
        for (int i = 0; i != (int)image_data.size(); i++) {
            ofs << image_data[i];
        }
    } else {
        throw std::runtime_error("Couldn't open image file");
    }
    return 0;
}