all: ex7

ex7: ex7.o
	gcc ex7.o -o ex7 -lpthread

ex7.o: ex7.c
	gcc -c ex7.c

run: ex7
	./ex7 $(BEGIN) $(END) $(H) $(NP)

clean:
	rm -rf *.o ex7
