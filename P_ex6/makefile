all: task6

task6: task6.o
	gcc task6.o -o task6 -lpthread

task6.o: task6.c
	gcc -c task6.c

run: task6
	./task6 $(NP) $(N) 

clean:
	rm -rf *.o task6
