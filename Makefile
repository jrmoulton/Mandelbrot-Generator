
# https://stackoverflow.com/questions/30573481/path-include-and-src-directory-makefile/30602701
#
# Compiler flags
#
CC = g++
CPPFLAGS = -std=gnu++14 -Wall -Werror -Wextra -Wshadow

#
# Project files
#
ftype = cpp
SRCDIR = src
SRCS = $(wildcard $(SRCDIR)/*.$(ftype))
EXE  = main
# ifeq ($(OS),Windows_NT) 
# 	EXE = $(EXE).exe 
# endif


#
# Debug build settings
#
DBGDIR = target/debug
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(patsubst $(SRCDIR)/%.$(ftype), $(DBGDIR)/%.o, $(SRCS))
DBGCFLAGS = -g -O0 -DDEBUG

#
# Release build settings
#
RELDIR = target/release
RELEXE = $(RELDIR)/$(EXE)
RELOBJS = $(patsubst $(SRCDIR)/%.$(ftype), $(RELDIR)/%.o, $(SRCS))
RELCFLAGS = -O3 -DNDEBUG

.PHONY: all clean debug release remake run

#
# Default build
#
all: release

#
# Debug rules
#
debug: $(DBGEXE)

$(DBGEXE): $(DBGOBJS) | $(DBGDIR)
	$(CC) $(CPPFLAGS) $(DBGCFLAGS) $^ -o $@

$(DBGDIR)/%.o: $(SRCDIR)/%.$(ftype) $(SRCDIR)/$(wildcard *.h) | $(DBGDIR)
	$(CC) $(CPPFLAGS) $(DBGCFLAGS) -c $< -o $@

#
# Release rules
#
release: $(RELEXE)

$(RELEXE): $(RELOBJS) | $(RELDIR)
	$(CC) $(CPPFLAGS) $(RELCFLAGS) $^ -o $@

$(RELDIR)/%.o: $(SRCDIR)/%.$(ftype) $(SRCDIR)/$(wildcard *.h) | $(RELDIR)
	$(CC) $(CPPFLAGS) $(RELCFLAGS) -c $< -o $@

#
# Other rules
#
clean:
	rm -rf $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS) $(DBGDIR) $(RELDIR) $(wildcard *.dSYM)

remake: clean all

run: release
	./$(RELEXE) 

$(RELDIR) $(DBGDIR):
	@"mkdir" -p "$@"

-include $(OBJ:.o=.d)
