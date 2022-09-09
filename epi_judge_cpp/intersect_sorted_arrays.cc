#include <vector>

#include "test_framework/generic_test.h"
using std::vector;

// Space complexity is O(1)
// Time complexity is O(mlogn)
// Where m is the size of the smaller array and n is the size of the bigger
// array.
std::vector<int> IntersectTwoSortedArrays1(const std::vector<int> &A,
                                           const std::vector<int> &B) {
  auto result = std::vector<int>{};
  if ((A.empty()) || (B.empty()))
    return {};

  const auto aSize = A.size();
  const auto bSize = B.size();
  result.reserve(aSize + bSize);

  const std::vector<int> *minArray = nullptr;
  const std::vector<int> *otherArray = nullptr;

  if (aSize < bSize) {
    minArray = &A;
    otherArray = &B;
  } else {
    minArray = &B;
    otherArray = &A;
  }

  size_t resultIdx = 0;
  for (const auto el : *minArray) {
    if (std::binary_search(otherArray->begin(), otherArray->end(), el)) {
      if ((resultIdx == 0) || (result.at(resultIdx - 1) != el)) {
        result.emplace_back(el);
        ++resultIdx;
      }
    }
  }

  return result;
}

// Space complexity is O(1)
// Time complexity is O(m + n)
// Where m is the size of the smaller array and n is the size of the bigger
// array.
std::vector<int> IntersectTwoSortedArrays(const std::vector<int> &A,
                                          const std::vector<int> &B) {
  auto result = std::vector<int>{};
  if ((A.empty()) || (B.empty()))
    return {};

  const auto aSize = A.size();
  const auto bSize = B.size();
  result.reserve(aSize + bSize);

  size_t aIdx = 0;
  size_t bIdx = 0;
  auto currentEl = A.at(aIdx);

  while ((aIdx < aSize) && (bIdx < bSize)) {
    for (; bIdx < bSize; ++bIdx) {
      const auto el = B.at(bIdx);
      if (el > currentEl) {
        currentEl = el;
        break;
      }
      if (el < currentEl)
        continue;

      if ((bIdx == 0) || (B.at(bIdx - 1) != el))
        result.emplace_back(el);
    }

    for (; (bIdx < bSize) && (aIdx < aSize); ++aIdx) {
      const auto el = A.at(aIdx);
      if (el > currentEl) {
        currentEl = el;
        break;
      }
      if (el < currentEl)
        continue;

      if ((aIdx == 0) || (A.at(aIdx - 1) != el))
        result.emplace_back(el);
    }
  }

  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "B"};
  return GenericTestMain(
      args, "intersect_sorted_arrays.cc", "intersect_sorted_arrays.tsv",
      &IntersectTwoSortedArrays, DefaultComparator{}, param_names);
}
