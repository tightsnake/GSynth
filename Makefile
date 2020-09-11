INC = -IC:/MySDL/include
LIB = -LC:/MySDL/lib
FLAGS = -lmingw32 -lSDL2main -lSDL2 -g -Wall

GSynth: main.o mixer.o
	gcc main.o mixer.o -o GSynth $(INC) $(LIB) $(FLAGS)

mixer.o: mixer.c mixer.h
	gcc -c mixer.c mixer.h $(INC) $(LIB) $(FLAGS)

main.o: main.c mixer.h
	gcc -c main.c mixer.h $(INC) $(LIB) $(FLAGS)

clean:
	del *.o *.exe *.sw*

new:
	mingw32-make clean
	mingw32-make GSynth
