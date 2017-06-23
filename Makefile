# . Makefile

pngver = 0.2.9

# CFLAGS  = -O2 -fPIC -c -W -Wall -pedantic -I$(PDYNAMO_PCORE)/extensions/cinclude -I../cinclude
CFLAGS = -Ipng++/png++-${pngver} -g
CC  = g++


default: mrtp

mrtp:  main.o  actors.o  vector.o  world.o  camera.o  color.o  buffer.o  parser.o  fontdata.o  png++/png++-${pngver}
	$(CC)  $(CFLAGS)  main.o  actors.o  vector.o  world.o  camera.o  color.o  buffer.o  parser.o  fontdata.o  -lpng  -lm  -o  mrtp

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

actors.o: actors.cpp actors.hpp
	$(CC) $(CFLAGS) -c actors.cpp -o actors.o

vector.o: vector.cpp vector.hpp
	$(CC) $(CFLAGS) -c vector.cpp -o vector.o

world.o: world.cpp world.hpp
	$(CC) $(CFLAGS) -c world.cpp -o world.o

camera.o: camera.cpp camera.hpp
	$(CC) $(CFLAGS) -c camera.cpp -o camera.o

color.o: color.cpp color.hpp
	$(CC) $(CFLAGS) -c color.cpp -o color.o

buffer.o: buffer.cpp buffer.hpp
	$(CC) $(CFLAGS) -c buffer.cpp -o buffer.o

parser.o: parser.cpp parser.hpp
	$(CC) $(CFLAGS) -c parser.cpp -o parser.o

fontdata.o: fontdata.cpp fontdata.hpp
	$(CC) $(CFLAGS) -c fontdata.cpp -o fontdata.o

png++/png++-${pngver}:
	+$(MAKE) -C png++

clean:
	if [ -e mrtp       ] ; then rm mrtp        ; fi
	if [ -e main.o     ] ; then rm main.o      ; fi
	if [ -e actors.o   ] ; then rm actors.o    ; fi
	if [ -e vector.o   ] ; then rm vector.o    ; fi
	if [ -e world.o    ] ; then rm world.o     ; fi
	if [ -e camera.o   ] ; then rm camera.o    ; fi
	if [ -e color.o    ] ; then rm color.o     ; fi
	if [ -e buffer.o   ] ; then rm buffer.o    ; fi
	if [ -e parser.o   ] ; then rm parser.o    ; fi
	if [ -e fontdata.o ] ; then rm fontdata.o  ; fi

clean_all: clean
	+$(MAKE) -C png++ clean
