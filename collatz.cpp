#include <thread>
#include <functional>
#include "collatz.h"

const big_int upperLimit = 2e7;
const int N_THREADS = 4;

#ifdef SERIAL
void serialCollatz() {
	int maxPathLength = 0;
  big_int maxN;
  maxCollatzPath(2, upperLimit, maxPathLength, maxN);
  printMax(maxPathLength, maxN);
}
#endif

#ifdef THREADED
void threadedCollatz() {
	int pathLengths[N_THREADS]; 
	big_int maxNs[N_THREADS];
  big_int nPerThread = big_int(upperLimit/N_THREADS);
	
  // Create and run threads
  std::thread threads[N_THREADS];
	for(int i=0; i<N_THREADS; ++i) {
		threads[i] = std::thread(maxCollatzPath, i*nPerThread, (i+1)*nPerThread, 
        std::ref(pathLengths[i]), std::ref(maxNs[i]));
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
#endif

int main(int argc, char** args) {
#ifdef SERIAL
  serialCollatz();
#endif
#ifdef THREADED
  threadedCollatz();
#endif
}


