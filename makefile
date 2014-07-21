cc = g++ --std=c++0x
THREAD_LIBS = -lboost_thread -lboost_system

all: collatzWithThreading collatz collatzWithCUDA

collatz: collatz.cpp
	$(cc) -o collatz collatz.cpp

collatzWithThreading: collatzWithThreading.cpp
	$(cc) -o collatzWithThreading collatzWithThreading.cpp $(THREAD_LIBS)

collatzWithCUDA: collatzWithCUDA.cpp
	touch collatzWithCUDA
