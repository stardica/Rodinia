CC = g++
SRC = pathfinder.cpp
EXE = pathfinder
FLAGS = -fopenmp

all: release

release:
	$(CC) $(SRC) $(FLAGS) -o $(EXE) -static -lpthread -ldl -lrt

debug:
	$(CC) $(SRC) -g -Wall -o $(EXE) -static -lpthread -ldl -lrt

clean:
	rm -f pathfinder