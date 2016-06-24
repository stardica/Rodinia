# Example
# target: dependencies
	# command 1
	# command 2
          # .
          # .
          # .
	# command n

# link objects(binaries) together

all: myocyte

myocyte:	main.o
	gcc	main.o \
			-m32 -lm -fopenmp \
	-o myocyte -static

# compile main function file into object (binary)
main.o: 	main.c \
				define.c \
				ecc.c \
				cam.c \
				fin.c \
				master.c \
				embedded_fehlberg_7_8.c \
				solver.c \
				file.c \
				timer.c
	gcc	main.c \
			-m32 -c -O3 -fopenmp -static

# delete all object files
clean:
	rm *.o myocyte output.txt
