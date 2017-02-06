CC=g++
CFLAGS=--std=c++0x
INCLUDES=collatz.h

all: threaded_collatz collatz

collatz: collatz.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -o $@ $<

threaded_collatz: threaded_collatz.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -pthread -o $@ $<

clean:
	rm collatz threaded_collatz
