# Makefile

ifndef RORAIMA_DIR
RORAIMA_DIR = ./
endif

BIN_DIR = $(RORAIMA_DIR)bin/

CXX = g++-4.7.2
CFLAGS = -O3 -march=native -mtune=native -funroll-loops -fPIC -Wall -std=c++11
CPPFLAGS = -I$(RORAIMA_DIR)kernel/
LIBS = -lgflags

CXXSOURCE = $(RORAIMA_DIR)driver/quora_online.cpp
CXXSOURCE_BF = $(RORAIMA_DIR)driver/quora_brute_force.cpp


TESTSOURCE = $(RORAIMA_DIR)test/test_build.cpp

MKDIR = \
	mkdir -p $(BIN_DIR)
RM = rm -rf

all: quora_online quora_brute_force 

# rules
quora_online: $(CXXSOURCE)
	$(MKDIR)
	$(CXX) $(CPPFLAGS) $(CFLAGS) $(LIBS) -o $(BIN_DIR)quora_online $^
	@echo 'Successfully Built in ./bin/quora_online'

quora_brute_force: $(CXXSOURCE_BF)
	$(MKDIR)
	$(CXX) $(CPPFLAGS) $(CFLAGS) $(LIBS) -o $(BIN_DIR)quora_brute_force $^
	@echo 'Successfully Built in ./bin/quora_brute_force'

.PHONY: clean

clean:
	$(RM) $(BIN_DIR)
