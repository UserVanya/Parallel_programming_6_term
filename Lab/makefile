all: lab

lab: lab.o
	mpicc lab.o -o lab -lm -lrt

lab.o: lab.c
	mpicc -c lab.c

run1: lab
	mpirun -n 1 ./lab

run2: lab
	mpirun -n 2 ./lab

run3: lab
	mpirun -n 3 ./lab

run4: lab
	mpirun -n 4 ./lab

clean:
	rm -rf *.o lab
