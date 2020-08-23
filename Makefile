INC = -IC:/MySDL/include
LIB = -LC:/MySDL/lib
FLAGS = -lmingw32 -lSDL2main -lSDL2 -g

GSynth: main.o
	gcc main.o -o GSynth $(INC) $(LIB) $(FLAGS)

main.o: main.c
	gcc -c main.c $(INC) $(LIB) $(FLAGS)

clean:
	del *.o *.exe *.sw*
