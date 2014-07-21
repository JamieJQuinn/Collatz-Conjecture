#include <iostream>
#include <vector>
#include <boost/thread.hpp>

using namespace std;

// Determines path length for Collatz sequence from n to 1
int collatz(unsigned long long n) {
	int count=0;
	while(n>1) {
		// check that n won't wrap around
		if(n*3+1<n) return -1;

		// if n divides by 2, divide by 2, else times by 3 and add 1
		if(n%2) n=n*3+1;
		else n/=2;
		++count;
	}
	return count;
}

// Gets a number n from the user (from args, otherwise asking)
unsigned int getN(int argc=0, char* argv[]=NULL) {
	unsigned int n;
	if(argc==1) {
		cout << "Please enter n: ";
		cin >> n;
		cout << endl;
	} else {
		n = stoi(argv[1]);
	}

	if(n<=1) {
		cout << "n must be greater than 1!" << endl;
		return getN(argc, argv);
	}

	return n;
}

/*class Collatz {
	public:
		void operator () (unsigned long long n, int *count) {
			*count=0;
			while(n>1) {
				// check that n won't wrap around
				if(n*3+1<n) {
					*count = -1;
					break;
				}

				// if n divides by 2, divide by 2, else times by 3 and add 1
				if(n%2) n=n*3+1;
				else n/=2;
				++*count;
			}
		}
};*/

// Finds the max path length for all n between N and M
void maxCollatz(
		unsigned long long N, 
		unsigned long long M, 
		int *maxCount,
		unsigned long long *maxN
		) {

	if(!maxN) return;
	if(!maxCount) return;
	if(M<N) return;

	*maxCount=0;
	int count=0;
	*maxN=0;
	std::vector<unsigned long long> roofHits;

	for(unsigned long long int i = N; i<M; ++i) {
		count = collatz(i);
		if(count == -1) {
			roofHits.push_back(i);
		}
		if ( *maxCount < count ) {
			*maxCount=count;
			*maxN=i;
		}
	}
}

void threadedMaxCollatz (unsigned long long N) {
	int counts[4]; 
	unsigned long long maxNs[4];
	
	// initialise 4 threads, each with a 1/4 of N
	boost::thread * threads[4];
	for(int i=0; i<4; ++i) {
		threads[i] = new boost::thread(
				maxCollatz, 
				i*int(N/4), 
				(i+1)*int(N/4), 
				counts+i,
				maxNs +i
				);
	}

	int maxCount = 0;
	unsigned long long maxN = 0;

	for(int i=0; i<4; ++i) {
		threads[i]->join();
		if(counts[i]>maxCount) {
			maxCount = counts[i];
			maxN = maxNs[i];
		}
	}

	cout << "Max collatz under " << N << " is " 
		 << maxCount << " at n=" << maxN << endl;
}

int main(int argc, char** args) {
	//cout << collatz(getN(argc, args)) << endl;
	//cout << collatz(7579309213675935) << endl; 
	//maxCollatz(7579309213675935);
	threadedMaxCollatz(10000000);
}
