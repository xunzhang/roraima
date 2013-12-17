# Makefile

ifndef RORAIMA_DIR
RORAIMA_DIR = ./
endif

BIN_DIR = $(RORAIMA_DIR)bin/

CXX = g++
CFLAGS = -O2 -march=native -mtune=native -funroll-loops -fPIC -Wall -std=c++11
CPPFLAGS = -I$(RORAIMA_DIR)kernel/
LIBS = -lgflags

CXXSOURCE = $(RORAIMA_DIR)driver/quora.cpp
TESTSOURCE = $(RORAIMA_DIR)test/test_build.cpp

MKDIR = \
	mkdir -p $(BIN_DIR)
RM = rm -rf

# rules
all: quora 

quora: $(CXXSOURCE)
	$(MKDIR)
	$(CXX) $(CPPFLAGS) $(CFLAGS) $(LIBS) -o $(BIN_DIR)quora $^
	@echo 'Successfully Built in ./bin/quora'

.PHONY: clean

clean:
	$(RM) $(BIN_DIR)
