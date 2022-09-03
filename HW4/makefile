all: ex4

ex4: ex4.o
	mpicc ex4.o -o ex4 -lm

ex4.o: ex4.c
	mpicc -c ex4.c

run: ex4
	mpirun -n 4 ./ex4 5000

clean:
	rm -rf *.o ex4
