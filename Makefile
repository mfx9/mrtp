# . Makefile for mrtp

PNGVER  = 0.2.9
INCLUDE = ./include
SRC     = ./src
CFLAGS  = -I${INCLUDE} -Ipng++/png++-${PNGVER} -O2 -W -Wall -pedantic
CC      = g++

OBJ = ${SRC}/main.o     \
      ${SRC}/actors.o   \
      ${SRC}/vector.o   \
      ${SRC}/world.o    \
      ${SRC}/camera.o   \
      ${SRC}/color.o    \
      ${SRC}/buffer.o   \
      ${SRC}/parser.o   \
      ${SRC}/fontdata.o \
      ${SRC}/texture.o


default: mrtp

mrtp: ${OBJ}
	$(CC) $(CFLAGS) ${OBJ} -lpng -lm -o mrtp

${SRC}/main.o: ${SRC}/main.cpp
	$(CC) $(CFLAGS) -c ${SRC}/main.cpp -o ${SRC}/main.o

${SRC}/actors.o: ${SRC}/actors.cpp ${INCLUDE}/actors.hpp
	$(CC) $(CFLAGS) -c ${SRC}/actors.cpp -o ${SRC}/actors.o

${SRC}/vector.o: ${SRC}/vector.cpp ${INCLUDE}/vector.hpp
	$(CC) $(CFLAGS) -c ${SRC}/vector.cpp -o ${SRC}/vector.o

${SRC}/world.o: ${SRC}/world.cpp ${INCLUDE}/world.hpp
	$(CC) $(CFLAGS) -c ${SRC}/world.cpp -o ${SRC}/world.o

${SRC}/camera.o: ${SRC}/camera.cpp ${INCLUDE}/camera.hpp
	$(CC) $(CFLAGS) -c ${SRC}/camera.cpp -o ${SRC}/camera.o

${SRC}/color.o: ${SRC}/color.cpp ${INCLUDE}/color.hpp
	$(CC) $(CFLAGS) -c ${SRC}/color.cpp -o ${SRC}/color.o

${SRC}/buffer.o: ${SRC}/buffer.cpp ${INCLUDE}/buffer.hpp
	$(CC) $(CFLAGS) -c ${SRC}/buffer.cpp -o ${SRC}/buffer.o

${SRC}/parser.o: ${SRC}/parser.cpp ${INCLUDE}/parser.hpp
	$(CC) $(CFLAGS) -c ${SRC}/parser.cpp -o ${SRC}/parser.o

${SRC}/fontdata.o: ${SRC}/fontdata.cpp ${INCLUDE}/fontdata.hpp
	$(CC) $(CFLAGS) -c ${SRC}/fontdata.cpp -o ${SRC}/fontdata.o

${SRC}/texture.o: ${SRC}/texture.cpp ${INCLUDE}/texture.hpp
	$(CC) $(CFLAGS) -c ${SRC}/texture.cpp -o ${SRC}/texture.o

clean:
	if [ -e mrtp ] ; then rm -v mrtp ; fi
	if [ -e  ${SRC}/main.o     ]  ;  then  rm -v  ${SRC}/main.o      ;  fi
	if [ -e  ${SRC}/actors.o   ]  ;  then  rm -v  ${SRC}/actors.o    ;  fi
	if [ -e  ${SRC}/vector.o   ]  ;  then  rm -v  ${SRC}/vector.o    ;  fi
	if [ -e  ${SRC}/world.o    ]  ;  then  rm -v  ${SRC}/world.o     ;  fi
	if [ -e  ${SRC}/camera.o   ]  ;  then  rm -v  ${SRC}/camera.o    ;  fi
	if [ -e  ${SRC}/color.o    ]  ;  then  rm -v  ${SRC}/color.o     ;  fi
	if [ -e  ${SRC}/buffer.o   ]  ;  then  rm -v  ${SRC}/buffer.o    ;  fi
	if [ -e  ${SRC}/parser.o   ]  ;  then  rm -v  ${SRC}/parser.o    ;  fi
	if [ -e  ${SRC}/fontdata.o ]  ;  then  rm -v  ${SRC}/fontdata.o  ;  fi
	if [ -e  ${SRC}/texture.o  ]  ;  then  rm -v  ${SRC}/texture.o   ;  fi
