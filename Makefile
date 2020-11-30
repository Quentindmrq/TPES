CC = gcc
CFLAGS = -Wall --std=c99
EXEC = test



%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
	
ES.o:
	gcc -o ES.o -c ES.c -Wall


test.o:
	gcc -o test.o -c test.c -Wall


test: test.o ES.o
	gcc -o test test.o ES.o

run:
	./test src.txt dest.txt

clean:
	rm -rf *.o
	rm -rf test
	rm -rf dest.txt
	touch dest.txt
