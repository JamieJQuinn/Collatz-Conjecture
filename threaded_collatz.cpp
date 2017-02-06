#include <thread>
#include <functional>
#include "collatz.h"

using namespace std;

#ifndef N_THREADS
#define N_THREADS 8
#endif

int main(int argc, char** args) {
  //big_int upperLimit = LARGEST_INT;
  big_int upperLimit = 2e7;

	int pathLengths[N_THREADS]; 
	big_int maxNs[N_THREADS];
  big_int nPerThread = big_int(upperLimit/N_THREADS);
	
  // Create and run threads
	thread threads[N_THREADS];
	for(int i=0; i<N_THREADS; ++i) {
		threads[i] = thread(maxCollatzPath, i*nPerThread, (i+1)*nPerThread, 
        ref(pathLengths[i]), ref(maxNs[i]));
	}

  // Get max out of all thread return values
	int maxPathLength = 0;
	big_int maxN = 0;
	for(int i=0; i<N_THREADS; ++i) {
		threads[i].join();
		if(pathLengths[i] > maxPathLength) {
			maxPathLength = pathLengths[i];
			maxN = maxNs[i];
		}
	}

  printMax(maxPathLength, maxN);
}
