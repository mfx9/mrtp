# . Makefile

INCLUDE=-I/usr/include/python2.7 -I/usr/include/eigen3 -I../png++/png++-0.2.9 -I.
FLAGS=-W -Wall -pedantic -fPIC -O2

Mrtp.so: Mrtp.o actor.o plane.o sphere.o cylinder.o texture.o light.o camera.o world.o renderer.o
	g++ -shared -o Mrtp.so Mrtp.o actor.o plane.o sphere.o cylinder.o texture.o light.o camera.o world.o renderer.o -lm -lpng

Mrtp.o: Mrtp.cpp
	g++ $(FLAGS) $(INCLUDE) -o Mrtp.o -c Mrtp.cpp

Mrtp.cpp: Mrtp.pyx Mrtp.pxd
	cython $(INCLUDE) -o Mrtp.cpp --cplus Mrtp.pyx

actor.o: actor.cpp actor.hpp
	g++ $(FLAGS) $(INCLUDE) -o actor.o -c actor.cpp

plane.o: plane.cpp plane.hpp
	g++ $(FLAGS) $(INCLUDE) -o plane.o -c plane.cpp

sphere.o: sphere.cpp sphere.hpp
	g++ $(FLAGS) $(INCLUDE) -o sphere.o -c sphere.cpp

cylinder.o: cylinder.cpp cylinder.hpp
	g++ $(FLAGS) $(INCLUDE) -o cylinder.o -c cylinder.cpp

texture.o: texture.cpp texture.hpp
	g++ $(FLAGS) $(INCLUDE) -o texture.o -c texture.cpp

light.o: light.cpp light.hpp
	g++ $(FLAGS) $(INCLUDE) -o light.o -c light.cpp

camera.o: camera.cpp camera.hpp
	g++ $(FLAGS) $(INCLUDE) -o camera.o -c camera.cpp

world.o: world.cpp world.hpp
	g++ $(FLAGS) $(INCLUDE) -o world.o -c world.cpp

renderer.o: renderer.cpp renderer.hpp
	g++ $(FLAGS) $(INCLUDE) -o renderer.o -c renderer.cpp

.PHONY: clean
clean:
	-rm -f Mrtp.cpp *.o *.so 2> /dev/null
