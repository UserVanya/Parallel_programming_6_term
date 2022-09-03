all: ex5

ex5: ex5.o
	gcc ex5.o -o ex5 -lpthread

ex5.o: ex5.c
	gcc -c ex5.c

run: ex5
	./ex5 $(NP) $(N)

clean:
	rm -rf *.o ex5
