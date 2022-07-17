CC = g++

HPP = headers/rgbo.hpp

CPP = main.cpp

OBJ = main.o

main: $(OBJ)
	$(CC) $(OBJ) -o main -lglut -lGLU -lGL -lX11

main.o: $(CPP) $(HPP)
	$(CC) -c $(CPP)

initialize:
	rm *.o *.exe main
