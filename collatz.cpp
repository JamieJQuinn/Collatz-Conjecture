#include <thread>
#include <functional>
#include <algorithm>
#include <iostream>
#include <vector>

#ifdef BOOST_COMPUTE_ENABLED
#include <boost/compute/algorithm/copy.hpp>
#include <boost/compute/algorithm/transform.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/algorithm/max_element.hpp>
#endif

typedef unsigned long long big_int;
const big_int UPPER_LIMIT = 1e7;
#ifndef N_THREADS
#define N_THREADS 4
#endif

int calcStoppingTime(big_int n) {
  // Determines path length for Collatz sequence from n to 1
	int stoppingTime=0;
	while(n>1) {
		// check that n won't wrap around
		if(n*3+1<n) return -1;

		// if n divides by 2, divide by 2, else times by 3 and add 1
		if(n%2) {
      n=n*3+1;
    } else {
      n/=2;
    }
		++stoppingTime;
	}
	return stoppingTime;
}

void calcMaxStoppingTime(
  big_int N, 
  big_int M, 
  // Return values
  int &maxStoppingTime,
  big_int &maxN
) {
  // Finds the max path length for all n between N and M
	if(M<N) return;

	maxStoppingTime=0;
	maxN=0;

#ifdef OMP_ENABLED
#pragma omp parallel for
#endif
  for(big_int i = N; i<M; ++i) {
		int stoppingTime = calcStoppingTime(i);
		if ( maxStoppingTime < stoppingTime ) {
			maxStoppingTime=stoppingTime;
			maxN=i;
		}
	}
}

void printMax(int maxStoppingTime, big_int maxN) {
	std::cout << "Max: " << maxStoppingTime 
    << " At: " << maxN 
    << std::endl;
}

void serial() {
	int maxStoppingTime = 0;
  big_int maxN;
  calcMaxStoppingTime(2, UPPER_LIMIT, maxStoppingTime, maxN);
  printMax(maxStoppingTime, maxN);
}

void openmp() {
  serial();
}

#ifdef THREADED
void threaded() {
	int stoppingTimes[N_THREADS]; 
	big_int maxNs[N_THREADS];
  big_int nPerThread = big_int(UPPER_LIMIT/N_THREADS);
	
  // Create and run threads
  std::thread threads[N_THREADS];
	for(int i=0; i<N_THREADS; ++i) {
		threads[i] = std::thread(calcMaxStoppingTime, i*nPerThread, (i+1)*nPerThread, 
        std::ref(stoppingTimes[i]), std::ref(maxNs[i]));
	}

  // Get max out of all thread return values
	int maxStoppingTime = 0;
	big_int maxN = 0;
	for(int i=0; i<N_THREADS; ++i) {
		threads[i].join();
		if(stoppingTimes[i] > maxStoppingTime) {
			maxStoppingTime = stoppingTimes[i];
			maxN = maxNs[i];
		}
	}

  printMax(maxStoppingTime, maxN);
}
#endif

#ifdef BOOST_COMPUTE_ENABLED
namespace compute = boost::compute;

BOOST_COMPUTE_FUNCTION(int, gpu_calc_stopping_time, (cl_ulong n), {
  // Determines path length for Collatz sequence from n to 1
	int stoppingTime=0;
	while(n>1) {
		// check that n won't wrap around
		if(n*3+1<n) return -1;

		// if n divides by 2, divide by 2, else times by 3 and add 1
		if(n%2) {
      n=n*3+1;
    } else {
      n/=2;
    }
		++stoppingTime;
	}
	return stoppingTime;
});

void boostCompute() {
  // Setup device
  compute::device device = compute::system::default_device();
  compute::context context(device);
  compute::command_queue queue(context, device);

  // Fill array with increasing values
  std::vector<cl_ulong> starting_points(UPPER_LIMIT);
  for(int i=0; i<UPPER_LIMIT; ++i) {
    starting_points[i] = i+1;
  }

  // Copy numbers over
  compute::vector<cl_ulong> device_vector(UPPER_LIMIT, context);
	compute::copy(
			starting_points.begin(), starting_points.end(), device_vector.begin(), queue
	);

  // Calculate stopping time on each value
  compute::transform(
      device_vector.begin(),
      device_vector.end(),
      device_vector.begin(),
      gpu_calc_stopping_time,
      queue
  );

  // Find max
  compute::vector<cl_ulong>::iterator max =
        compute::max_element(device_vector.begin(), device_vector.end(), queue);

  // Get max back
	compute::copy(max, max + 1, starting_points.begin(), queue);

  int maxStoppingTime = starting_points[0];
  int maxN = max - device_vector.begin() + 1;
  printMax(maxStoppingTime, maxN);
}
#endif

int main(int argc, char** args) {
#ifdef SERIAL
  serial();
#endif
#ifdef THREADED
  threaded();
#endif
#ifdef OMP_ENABLED
  openmp();
#endif
#ifdef BOOST_COMPUTE_ENABLED
  boostCompute();
#endif
}
