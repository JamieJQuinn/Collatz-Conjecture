#include <thread>
#include <functional>
#include <iostream>

typedef unsigned long long big_int;
const big_int LARGEST_INT = big_int(0) - 1;
const int OVERFLOW_ERROR = -1;
const big_int upperLimit = 2e7;
const int N_THREADS = 4;

int calcCollatzPathLength(big_int n) {
  // Determines path length for Collatz sequence from n to 1
	int pathLength=0;
	while(n>1) {
		// check that n won't wrap around
		if(n*3+1<n) return OVERFLOW_ERROR;

		// if n divides by 2, divide by 2, else times by 3 and add 1
		if(n%2) {
      n=n*3+1;
    } else {
      n/=2;
    }
		++pathLength;
	}
	return pathLength;
}

void maxCollatzPath(
  big_int N, 
  big_int M, 
  // Return values
  int &maxPathLength,
  big_int &maxN
) {
  // Finds the max path length for all n between N and M
	if(M<N) return;

	maxPathLength=0;
	maxN=0;

#ifdef OMP_ENABLED
#pragma omp parallel for
#endif
  for(big_int i = N; i<M; ++i) {
		int pathLength = calcCollatzPathLength(i);
		if ( maxPathLength < pathLength ) {
			maxPathLength=pathLength;
			maxN=i;
		}
	}
}

void printMax(int maxPathLength, big_int maxN) {
	std::cout << "Max: " << maxPathLength 
    << " At: " << maxN 
    << std::endl;
}

void serialCollatz() {
	int maxPathLength = 0;
  big_int maxN;
  maxCollatzPath(2, upperLimit, maxPathLength, maxN);
  printMax(maxPathLength, maxN);
}

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
#ifdef OMP_ENABLED
  serialCollatz();
#endif
}


