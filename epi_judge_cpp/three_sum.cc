#include <vector>

#include "test_framework/generic_test.h"
using std::vector;

bool HasTwoSum1(const std::vector<int> &A, const std::unordered_set<int> &numbersInTheList, int t)
{
  for (const auto el : A)
  {
    const auto otherEl = t - el;

    if (numbersInTheList.contains(otherEl))
      return true;
  }

  return false;
}

// Has a time complexity of O(n^2)
// Has a space complexity of O(n)
// Where n is the number of elements in the list.
bool HasThreeSum1(std::vector<int> A, int t)
{
  if (A.empty())
    return false;

  std::unordered_set<int> numbersInTheList{};
  for (const auto el : A)
    numbersInTheList.insert(el);

  for (const auto el : A)
  {
    const auto sumOfOtherTwoValues = t - el;
    if (HasTwoSum1(A, numbersInTheList, sumOfOtherTwoValues))
      return true;
  }

  return false;
}

/* *********************************************************************************** */

// Has a time complexity of O(n^2logn)
// Has a space complexity of O(1)
// Where n is the number of elements in the list.
bool HasThreeSum2(std::vector<int> A, int t)
{
    std::sort(A.begin(), A.end());
    for (const auto firstEl : A)
    {
        const auto twoSums = t - firstEl;
        for (const auto secondEl : A)
        {
            if (std::binary_search(A.begin(), A.end(), twoSums - secondEl))
                return true;
        }
    }

    return false;
}

/* *********************************************************************************** */

bool HasTwoSum(const std::vector<int> &sortedA, int t)
{
  auto startIdx = 0;
  int endIdx = sortedA.size() - 1;

  while (startIdx <= endIdx)
  {
    const auto sumOfCurrentTwoValues = sortedA.at(startIdx) + sortedA.at(endIdx); 

    if (sumOfCurrentTwoValues < t)
        startIdx++;
    else if (sumOfCurrentTwoValues > t)
        endIdx--;
    else
      return true;
  }

  return false;
}

// Has a time complexity of O(n^2)
// Has a space complexity of O(1)
// Where n is the number of elements in the list.
bool HasThreeSum(std::vector<int> A, int t)
{
  std::sort(A.begin(), A.end());
  return std::any_of(A.begin(), A.end(), [&](int el){ return HasTwoSum(A, t - el);});
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "t"};
  return GenericTestMain(args, "three_sum.cc", "three_sum.tsv", &HasThreeSum,
                         DefaultComparator{}, param_names);
}
