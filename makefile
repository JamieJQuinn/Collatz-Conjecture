CC=g++
CFLAGS=--std=c++0x
EXECS =serial threaded with_openmp

all: $(EXECS)

serial: collatz.cpp
	$(CC) $(CFLAGS) -DSERIAL -o $@ $<

threaded: collatz.cpp
	$(CC) $(CFLAGS) -DTHREADED -pthread -o $@ $<

with_openmp: collatz.cpp
	$(CC) $(CFLAGS) -DOMP_ENABLED -o $@ $< -fopenmp

clean:
	rm $(EXECS)
