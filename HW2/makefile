all: ex2

ex2: ex2.o
	mpicc --std=c99 ex2.o -o ex2 

ex2.o: ex2.c
	mpicc --std=c99 -c ex2.c

run: ex2
	mpirun -n 4 ./ex2 121

clean:
	rm -rf *.o ex2
