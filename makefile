CC=g++
CFLAGS=--std=c++0x
INCLUDES=collatz.h

all: serial threaded

serial: collatz.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -DSERIAL -o $@ $<

threaded: collatz.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -DTHREADED -pthread -o $@ $<

clean:
	rm serial threaded
