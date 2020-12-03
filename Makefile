CC = gcc
CFLAGS = -Wall --std=c99 -g
EXEC = test

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
	

test: test.o stdes.o
	gcc -o test test.o stdes.o $(CFLAGS)
	
rand: test-rand.o stdes.o
	$(CC) -o rand test-rand.o stdes.o $(CFLAGS)

fecriref : test_fecriref.o stdes.o
	$(CC) -o fecriref test_fecriref.o stdes.o $(CFLAGS)

fliref : test_fliref.o stdes.o
	$(CC) -o fliref test_fliref.o stdes.o $(CFLAGS)

run:
	./test src.txt dest.txt

runf:
	./fecriref

runfliref:
	./fliref
	
createlib:
	$(CC) -c -fPIC stdes.c
	$(CC) -shared -o libstdes.so.1 stdes.o
	

testformat:
	$(CC) -c test-format.c $(CFLAGS)
	$(CC) -o test-format test-format.o libstdes.so.1 $(CFLAGS)
	export LD_LIBRARY_PATH=.

runrand:
	./rand

clean:
	rm -rf *.o
	rm -rf test
	rm -rf fecriref
	rm -rf fliref
	rm -rf dest.txt
	touch dest.txt
	rm -rf rand-file.txt
	touch rand-file.txt
