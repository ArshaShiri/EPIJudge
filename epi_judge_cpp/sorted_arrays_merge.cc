#include <vector>

#include "test_framework/generic_test.h"
using std::vector;

// This itself can also be used as a solution to this problem.
// The space complexity is O(n) and time complexity is O(nlogn)
// where n is the number of elements in all the arrays combined.
vector<int> MergeSortedArrays1(const vector<vector<int>> &sorted_arrays) {
  auto result = std::vector<int>{};

  for (const auto &sortedArray : sorted_arrays)
    result.insert(result.end(), sortedArray.begin(), sortedArray.end());

  std::sort(result.begin(), result.end());
  return result;
}

struct ArrayInfo {
  bool operator>(const ArrayInfo &other) const { return el > other.el; }

  ArrayInfo(ArrayInfo&&) noexcept = default;
  ArrayInfo(const ArrayInfo&) = default;

  ArrayInfo& operator=(ArrayInfo&&) noexcept = default;
  ArrayInfo& operator=(const ArrayInfo&) = default;

  const vector<int> *array;
  size_t arraySize{0};
  int arrayIdx{0};
  int el{0};
};

vector<int> MergeSortedArrays(const vector<vector<int>> &sorted_arrays) {
  if (sorted_arrays.empty())
    return {};

  const auto numberOfSortedArrays = sorted_arrays.size();
  if (numberOfSortedArrays == 1)
    return sorted_arrays.at(0);

  size_t totalSize = 0;
  std::for_each(sorted_arrays.begin(), sorted_arrays.end(),
                [&totalSize](const auto &array) { totalSize += array.size(); });

  auto result = std::vector<int>{};
  result.reserve(totalSize);

  std::priority_queue<ArrayInfo, std::vector<ArrayInfo>,
                      std::greater<ArrayInfo>>
      minHeap{};

  std::for_each(
      sorted_arrays.begin(), sorted_arrays.end(),
      [&minHeap](const auto &array) {
        if (!array.empty())
          minHeap.emplace(ArrayInfo{&array, array.size(), 0, array.at(0)});
      });

  while (!minHeap.empty()) {
    const auto top = minHeap.top();
    result.emplace_back(top.el);
    minHeap.pop();

    const auto nextArrayIdx = top.arrayIdx + 1;
    if (nextArrayIdx < top.arraySize)
      minHeap.emplace(ArrayInfo{top.array, top.arraySize, nextArrayIdx,
                                top.array->at(nextArrayIdx)});
  }

  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sorted_arrays"};
  return GenericTestMain(args, "sorted_arrays_merge.cc",
                         "sorted_arrays_merge.tsv", &MergeSortedArrays,
                         DefaultComparator{}, param_names);
}
