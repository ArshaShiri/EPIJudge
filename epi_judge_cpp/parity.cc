#include "test_framework/generic_test.h"

#include <cmath>

// O(n) where n is the number of bits.
short ParityInitialSolution(unsigned long long x) {
  auto numberOfOneBits = 0;

  while (x != 0) {
    numberOfOneBits += x & 0b1;
    x >>= 1;
  }

  return numberOfOneBits % 2 == 0 ? 0 : 1;
}

// O(n) where n is the number of bits.
short ParityNoModulusOperator(unsigned long long x) {
  auto parity = 0;

  while (x != 0) {
    parity ^= x & 0b1;
    x >>= 1;
  }

  return parity;
}

// O(k) where k is the number of set bits.
short ParityLessAndOperators(unsigned long long x) {
  auto parity = 0;

  while (x != 0) {
    x &= (x - 1);
    parity ^= 1;
  }

  return parity;
}

// Optima solution if we want to calculate the parity of many 64-bit numbers:
// Calculate the parity of all 16 bit numbers.
constexpr size_t sizeOfArray = static_cast<size_t>(std::pow(2, 16));
auto preCalculatedValues = std::array<short, sizeOfArray>{};

void fillPreCalculatedValues() {
  for (auto idx = 0; idx < sizeOfArray; ++idx) {
    preCalculatedValues.at(idx) = ParityLessAndOperators(idx);
  }
}

short ParityEfficientForManyCalculations(unsigned long long x) {
  const auto groupSize = 16;
  const auto bitMask = 0xffff;

  return preCalculatedValues.at(x >> (3 * groupSize)) ^
         preCalculatedValues.at((x >> (2 * groupSize)) & bitMask) ^
         preCalculatedValues.at((x >> groupSize) & bitMask) ^
         preCalculatedValues.at(x & bitMask);
}

short Parity(unsigned long long x)
{
  x ^= x >> 32;
  x ^= x >> 16;
  x ^= x >> 8;
  x ^= x >> 4;
  x ^= x >> 2;
  x ^= x >> 1;

  return x & 0b1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};

  fillPreCalculatedValues();

  return GenericTestMain(args, "parity.cc", "parity.tsv", &Parity,
                         DefaultComparator{}, param_names);
}
