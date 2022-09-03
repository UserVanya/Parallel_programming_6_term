all: run

run: test
	mpirun -n 4 ./test

test: test.o
	mpicc -std=c99 test.o -o test

test.o: test.c
	mpicc -std=c99 -c test.c

clean:
	rm -rf *.o test
