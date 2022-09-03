all: ex3

ex3: ex3.o
	mpicc ex3.o -o ex3

ex3.o: ex3.c
	mpicc -c ex3.c

run: ex3
	mpirun -n 1 ./ex3 0

clean:
	rm -rf *.o ex3
