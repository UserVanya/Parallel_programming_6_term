all: lab_add

lab_add: lab_add.o
	mpicc lab_add.o -o lab_add -lm -lrt

lab_add.o: lab_add.c
	mpicc -c lab_add.c

run: lab_add
	mpirun -n $(NP) ./lab_add $(P) $(M)

clean:
	rm -rf *.o lab_add
