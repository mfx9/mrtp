# Makefile for main.so

INCLUDE=-I/usr/include/python2.7 -I/usr/include/eigen3 -I/usr/include/png++ -I../include
LIBS=-L../src
FLAGS=-W -Wall -pedantic -fPIC -O2


main.so: main.o ../src/libmrtp.a
	g++ $(LIBS) -shared main.o -o main.so -fopenmp -lm -lpng -lmrtp

main.o: main.cpp
	g++ $(FLAGS) $(INCLUDE) -o main.o -c main.cpp

main.cpp: main.pyx main.pxd
	cython $(INCLUDE) -o main.cpp --cplus main.pyx

../src/libmrtp.a:
	+$(MAKE) -C ../src

.PHONY: clean
clean:
	-rm -f main.cpp *.o *.so &>/dev/null
