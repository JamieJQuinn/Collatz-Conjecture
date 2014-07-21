#include <iostream>
#include <vector>

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

// Finds the max path length for all n under N
void maxCollatz(unsigned long long int N= 2147483647) {
	int maxCount=0, count=0;
	unsigned long long int maxN;
	std::vector<unsigned long long> roofHits;

	for(unsigned long long int i = 2; i<N; ++i) {
		if(i%1000000==0) cout << double(i)/double(N)*100 << "% complete" << endl;
		count = collatz(i);
		if(count == -1) {
			roofHits.push_back(i);
		}
		if ( maxCount < count ) {
			maxCount=count;
			maxN=i;
		}
	}

	cout << "Max collatz within " << N << " is " 
		 << maxCount << " at n=" << maxN << endl;

	for(int i=0; i<roofHits.size(); ++i) {
		cout << "Roof hit at " << roofHits[i] << endl;
	}
}

int main(int argc, char** args) {
	//cout << collatz(getN(argc, args)) << endl;
	//cout << collatz(7579309213675935) << endl; 
	//maxCollatz(7579309213675935);
	maxCollatz(10000000);
}
