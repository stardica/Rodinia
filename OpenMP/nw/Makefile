# C compiler
CC = g++
CC_FLAGS = -std=c++0x -g -fopenmp -m32 

all: needle

needle: 
	$(CC) $(CC_FLAGS) -I/home/stardica/Dropbox/CDA7919DoctoralResearch/IntelPCM/src/ needle.cpp -o needle -L/home/stardica/Dropbox/CDA7919DoctoralResearch/IntelPCM/Release_Static/ -static -lpthread -ldl -lrt -lIntelPCM

clean:
	rm -f needle 
