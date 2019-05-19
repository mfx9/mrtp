# Makefile for mrtp_cli

INCLUDE=-I/usr/include/eigen3 -I/usr/include/png++ -I../include -I../cpptoml/include
FLAGS=-W -Wall -pedantic -fPIC -O2


mrtp_cli: main.o actor.o plane.o sphere.o cylinder.o texture.o light.o camera.o world.o renderer.o
	g++ $^ -o $@ -fopenmp -lm -lpng

main.o: main.cpp
	g++ $(FLAGS) $(INCLUDE) -o main.o -c main.cpp

actor.o: actor.cpp ../include/actor.hpp
	g++ $(FLAGS) $(INCLUDE) -o actor.o -c actor.cpp

plane.o: plane.cpp ../include/plane.hpp
	g++ $(FLAGS) $(INCLUDE) -o plane.o -c plane.cpp

sphere.o: sphere.cpp ../include/sphere.hpp
	g++ $(FLAGS) $(INCLUDE) -o sphere.o -c sphere.cpp

cylinder.o: cylinder.cpp ../include/cylinder.hpp
	g++ $(FLAGS) $(INCLUDE) -o cylinder.o -c cylinder.cpp

texture.o: texture.cpp ../include/texture.hpp
	g++ $(FLAGS) $(INCLUDE) -o texture.o -c texture.cpp

light.o: light.cpp ../include/light.hpp
	g++ $(FLAGS) $(INCLUDE) -o light.o -c light.cpp

camera.o: camera.cpp ../include/camera.hpp
	g++ $(FLAGS) $(INCLUDE) -o camera.o -c camera.cpp

world.o: world.cpp ../include/world.hpp
	g++ $(FLAGS) $(INCLUDE) -o world.o -c world.cpp

# Here -fopenmp is only used so _OPENMP is defined
renderer.o: renderer.cpp ../include/renderer.hpp
	g++ $(FLAGS) -fopenmp $(INCLUDE) -o renderer.o -c renderer.cpp

.PHONY: clean
clean:
	-rm -f mrtp_cli *.o &>/dev/null
