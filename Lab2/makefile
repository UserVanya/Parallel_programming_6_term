all: lab2

lab2: lab2.o
	mpicc lab2.o -o lab2 -lm -lrt

lab2.o: lab2.c
	mpicc -c lab2.c

run1: lab2
	mpirun -n 1 ./lab2

run2: lab2
	mpirun -n 2 ./lab2

run3: lab2
	mpirun -n 3 ./lab2

run4: lab2
	mpirun -n 4 ./lab2

clean:
	rm -rf *.o lab2
