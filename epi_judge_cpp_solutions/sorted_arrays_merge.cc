#include <algorithm>
#include <iterator>
#include <queue>
#include <vector>

#include "test_framework/generic_test.h"

using std::greater;
using std::next;
using std::priority_queue;
using std::vector;

vector<int> MergeSortedArrays1(const vector<vector<int>> &sorted_arrays) {
  auto result = std::vector<int>{};

  for (const auto &sortedArray : sorted_arrays)
    result.insert(result.end(), sortedArray.begin(), sortedArray.end());

  std::sort(result.begin(), result.end());
  return result;
}

vector<int> MergeSortedArrays(const vector<vector<int>>& sorted_arrays) {
  struct IteratorCurrentAndEnd {
    bool operator<(const IteratorCurrentAndEnd& that) const {
      return *current > *that.current;
    }

    vector<int>::const_iterator current, end;
  };
  priority_queue<IteratorCurrentAndEnd> min_heap;

  for (const vector<int>& sorted_array : sorted_arrays) {
    if (!empty(sorted_array)) {
      min_heap.push({cbegin(sorted_array), cend(sorted_array)});
    }
  }

  vector<int> result;
  while (!empty(min_heap)) {
    auto [current, end] = min_heap.top();
    min_heap.pop();
    result.emplace_back(*current);
    if (next(current) != end) {
      min_heap.push({next(current), end});
    }
  }
  return result;
}

struct ArrayInfo
{
  bool operator>(const ArrayInfo &other) const { return el > other.el; }

  const vector<int> *array;
  size_t arraySize{ 0 };
  int arrayIdx{ 0 };
  int el{ 0 };
};

vector<int> MergeSortedArrays2(const vector<vector<int>> &sorted_arrays)
{
  if (sorted_arrays.empty())
    return {};

  const auto numberOfSortedArrays = sorted_arrays.size();
  if (numberOfSortedArrays == 1)
    return sorted_arrays.at(0);

  size_t totalSize = 0;
  std::for_each(
    sorted_arrays.begin(), sorted_arrays.end(), [&totalSize](const auto &array) { totalSize += array.size(); });

  auto result = std::vector<int>{};
  result.reserve(totalSize);

  std::priority_queue<ArrayInfo, std::vector<ArrayInfo>, std::greater<ArrayInfo>> minHeap{};

  std::for_each(sorted_arrays.begin(), sorted_arrays.end(), [&minHeap](const auto &array) {
    if (!array.empty())
      minHeap.emplace(ArrayInfo{ &array, array.size(), 0, array.at(0) });
  });

  while (!minHeap.empty())
  {
    const auto top = minHeap.top();
    result.emplace_back(top.el);
    minHeap.pop();

    const auto nextArrayIdx = top.arrayIdx + 1;
    if (nextArrayIdx < top.arraySize)
      minHeap.emplace(ArrayInfo{ top.array, top.arraySize, nextArrayIdx, top.array->at(nextArrayIdx) });
  }

  return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sorted_arrays"};
  return GenericTestMain(args, "sorted_arrays_merge.cc",
                         "sorted_arrays_merge.tsv", &MergeSortedArrays,
                         DefaultComparator{}, param_names);
}
