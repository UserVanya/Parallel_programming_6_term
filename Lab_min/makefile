all: min

min: min.o
	mpicc min.o -o min

min.o: min.c
	mpicc -c min.c

run: min
	mpirun -n 4 ./min

clean:
	rm -rf *.o min
