

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

/*
Summary: Generate and save an madelbrot set as a bmp image file.
Begin:
    Seed a random number generator and get a random number between 1 and 3
    set the bmp header array
    set the dib header array
    initialize variables
    loop over the columns:
        print a progress update
        loop over rows:
            scale values between mandelbrot range
            while value remains bounded up to 1000 iterations:
                calculate z^2
            color differently based on random selection choice
            push image data to vector
    open correect image file for choice
    if image opens correctly:
        wrtie the dib header
        write the dib header
        write the image date
    else if didn't open properly:
        throw error
End
*/
int main() {
    auto start = std::chrono::high_resolution_clock::now();
    // Seed a random number generator and get a random number between 1 and 3
    unsigned int seed =
        std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(1, 3);
    int choice = distribution(generator);
    choice = 2;

    // set the bmp header array
    std::array<unsigned char, 14> bmp = {
        0x42, 0x4D,              // BMP ID
        0x76, 0xF3, 0x89, 0x06,  // Total size of file
        0x00, 0x00, 0x00, 0x00,  // unused
        0x36, 0x00, 0x00, 0x00,  // offset where image starts
    };

    // set the dib header array
    std::array<unsigned char, 40> dib = {
        0x28, 0x00, 0x00, 0x00,  // number of bytes in dib header
        0x40, 0x1F, 0x00, 0x00,  // Length in pixels
        0xDB, 0x11, 0x00, 0x00,  // Height in pixels
        0x01, 0x00,              // number of color planes
        0x18, 0x00,              // number of bits per pixel
        0x00, 0x00, 0x00, 0x00,  // compression
        0x40, 0xF3, 0x89, 0x06,  // size of image data in bytes
        0x13, 0x0B, 0x00, 0x00,  // print resoolution
        0x00, 0x00, 0x00, 0x00,  // print resolution
        0x00, 0x00, 0x00, 0x00,
    };

    // initialize variables
    std::vector<unsigned char> image_data;
    unsigned char blue, green, red;
    double x0, y0, x, y, xtemp;
    int iter = 0, tempint;

    // Loop over the columns
    for (int i = 0; i < HEIGHT; i++) {
        if (i % 50 == 0) {
            // print a progress update
            std::cerr << "\rLines remaining: " << HEIGHT - i << " out of "
                      << HEIGHT << " " << std::flush;
        }
        // loop over rows:
        for (int j = 0; j < LENGTH; j++) {
            // scale values between mandelbrot range
            x0 = ((double)j * (1.0 - -2.5)) / LENGTH + -2.5;
            y0 = ((double)i * (1.0 - -1.0)) / HEIGHT + -1.0;
            x = 0.0;
            y = 0.0;
            iter = 0;
            // while value remains bounded up to 1000 iterations:
            while (x * x + y * y <= 4 && iter < ITERATIONS) {
                // calculate z^2
                xtemp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtemp;
                iter += 1;
            }
            // color differently based on random selection choice
            switch (choice) {
                case 1:
                    blue = std::sin(255 - ((double)iter / ITERATIONS)) * 255.0;
                    green = blue + (std::sin(blue) * 255);
                    red = std::cos(green) * 255;
                    break;
                case 2:
                    green = ((((double)iter / ITERATIONS)) * 255.0) * 3;
                    tempint = green * 4;
                    blue = tempint > 255 ? 255 : tempint;
                    red = std::sin(blue / 3) * 255;
                    break;
                case 3:
                    tempint = std::pow(((double)iter / ITERATIONS) * 255.0, 2);
                    blue = tempint;
                    green = blue;
                    red = blue;
                    break;
                case 4:
                    blue = std::sin(255 - ((double)iter / ITERATIONS)) * 255.0;
                    green = blue + (std::sin(blue) * 255);
                    red = green;
                    break;
            }
            // push image data to vector
            image_data.push_back(blue);
            image_data.push_back((unsigned char)green);
            image_data.push_back((unsigned char)red);
        }
    }
    std::cerr << "\rLines remaining: " << 0 << " out of " << HEIGHT << " "
              << std::flush << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << duration.count() << std::endl;

    // open correect image file for choice
    std::ofstream ofs("mandelbrot_" + std::to_string(choice) + ".bmp",
                      std::ios::out | std::ios::binary);

    // if image opens correctly:
    if (ofs.is_open()) {
        // wrtie the dib header
        for (auto i = bmp.begin(); i != bmp.end(); i++) {
            ofs << *i;
        }
        // write the dib header
        for (auto i = dib.begin(); i != dib.end(); i++) {
            ofs << *i;
        }
        // write the image date
        for (int i = 0; i != (int)image_data.size(); i++) {
            ofs << image_data[i];
        }
        // else if didn't open properly
    } else {
        // throw error
        throw std::runtime_error("Couldn't open image file");
    }
    return 0;
}