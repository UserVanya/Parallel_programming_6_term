all: lab3

lab3: lab3.o
	mpicc lab3.o -o lab3 -lm -lrt

lab3.o: lab3.c
	mpicc -c lab3.c

run: lab3
	mpirun -n $(NP) ./lab3 $(P) $(M)

clean:
	rm -rf *.o lab3
