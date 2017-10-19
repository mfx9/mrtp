# . Makefile for mrtp

PNGVER   = 0.2.9
CC_FLAGS = -Ipng++/png++-$(PNGVER) -W -Wall -pedantic -fopenmp
LD_FLAGS = -fopenmp
LIBS     = -lpng -lm 
CXX      = g++

CPP_FILES = $(wildcard *.cpp)
OBJ_FILES = $(CPP_FILES:.cpp=.o)


default: mrtp

mrtp: $(OBJ_FILES)
	$(CXX) $(LD_FLAGS) $^ $(LIBS) -o $@

%.o: %.cpp
	$(CXX) $(CC_FLAGS) -c $< -o $@

.PHONY: clean
clean:
	-rm -f mrtp *.o 2> /dev/null
