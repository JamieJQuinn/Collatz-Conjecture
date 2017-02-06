#ifndef __COLLATZ_H__
#define __COLLATZ_H__

#include <iostream>

typedef unsigned long long big_int;
const big_int LARGEST_INT = big_int(0) - 1;
const int OVERFLOW_ERROR = -1;

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

	for(big_int i = N; i<M; ++i) {
		int pathLength = calcCollatzPathLength(i);
    //if (pathLength == OVERFLOW_ERROR) {
      //std::cout << "Overflow from n=" << i << std::endl;
      //return;
    //}
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

#endif
