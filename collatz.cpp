#include "collatz.h"

int main(int argc, char** args) {
  //big_int upperLimit = LARGEST_INT;
  big_int upperLimit = 2e7;

	int maxPathLength = 0;
  big_int maxN;
  maxCollatzPath(2, upperLimit, maxPathLength, maxN);
  printMax(maxPathLength, maxN);
}
