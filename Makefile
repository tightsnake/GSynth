INC = -IC:/MySDL/include
LIB = -LC:/MySDL/lib
FLAGS = -lmingw32 -lSDL2main -lSDL2 -g -Wall

GSynth: main.o
	gcc main.o -o GSynth $(INC) $(LIB) $(FLAGS)

main.o: main.c gsynth.h mixer.h
	gcc -c main.c $(INC) $(LIB) $(FLAGS)

clean:
	del *.o *.exe *.sw*

new:
	mingw32-make clean
	mingw32-make GSynth
