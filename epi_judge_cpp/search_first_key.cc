#include <vector>

#include "test_framework/generic_test.h"
using std::vector;

// Use of STL. Space complexity is O(1)
// Time complexity is O(logn)
// If the provided container does not support 
// random-access iterators, time complexity can be O(n)
int SearchFirstOfK1(const vector<int>& A, int k) {
  if (A.empty())
    return -1;

  const auto valueIt = std::lower_bound(A.begin(), A.end(), k);
  
  if ((valueIt == A.end()) || (*valueIt != k))
    return -1;

  return std::distance(A.begin(), valueIt);
}

// Time complexity is O(logn)
int SearchFirstOfK(const std::vector<int>& A, int k) {
  int idx = -1;

  if (A.empty())
    return idx;

  int lowIdx = 0;
  int highIdx = A.size() - 1;

  while (lowIdx <= highIdx) {
    const auto currentIdx = lowIdx + (highIdx - lowIdx) / 2;
    const auto currentValue = A.at(currentIdx);

    if (currentValue < k)
        lowIdx = currentIdx + 1;
    else {
        highIdx = currentIdx - 1;

        if (currentValue == k)
            idx = currentIdx;
    }
  }

  return idx;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "k"};
  return GenericTestMain(args, "search_first_key.cc", "search_first_key.tsv",
                         &SearchFirstOfK, DefaultComparator{}, param_names);
}
