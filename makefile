CC=g++
CFLAGS=--std=c++11
EXES=serial threaded with_openmp with_boost_compute

all: $(EXES)

serial: collatz.cpp
	$(CC) $(CFLAGS) -DSERIAL -o $@ $<

threaded: collatz.cpp
	$(CC) $(CFLAGS) -DTHREADED -DN_THREADS=4 -pthread -o $@ $<

with_openmp: collatz.cpp
	$(CC) $(CFLAGS) -DOMP_ENABLED -o $@ $< -fopenmp

with_boost_compute: collatz.cpp
	$(CC) $(CFLAGS) -DBOOST_COMPUTE_ENABLED -o $@ $< -lOpenCL

clean:
	rm $(EXES)
