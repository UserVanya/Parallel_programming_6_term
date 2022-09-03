all: ex6

ex6: ex6.o
	gcc ex6.o -o ex6 -lpthread

ex6.o: ex6.c
	gcc -c ex6.c

run: ex6
	./ex6 $(N) $(NP)

clean:
	rm -rf *.o ex6
