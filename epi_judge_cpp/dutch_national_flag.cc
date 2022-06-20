#include <array>
#include <vector>

#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::vector;
enum class Color { kRed, kWhite, kBlue };

// O(n) time and space.
void DutchFlagPartition1(int pivot_index, vector<Color> *A_ptr) {
  if (A_ptr->empty())
    return;

  const auto pivot = A_ptr->at(pivot_index);

  auto colorsSmaller = vector<Color>{};
  auto colorsEqual = vector<Color>{};
  auto colorsBigger = vector<Color>{};

  for (const auto color : *A_ptr) {
    if (static_cast<int>(color) < static_cast<int>(pivot))
      colorsSmaller.push_back(color);
    else if (static_cast<int>(color) > static_cast<int>(pivot))
      colorsBigger.push_back(color);
    else
      colorsEqual.push_back(color);
  }

  auto dutchFlagColors = std::vector<Color>{};
  dutchFlagColors.reserve(A_ptr->size());

  dutchFlagColors.insert(dutchFlagColors.end(), colorsSmaller.begin(),
                         colorsSmaller.end());
  dutchFlagColors.insert(dutchFlagColors.end(), colorsEqual.begin(),
                         colorsEqual.end());
  dutchFlagColors.insert(dutchFlagColors.end(), colorsBigger.begin(),
                         colorsBigger.end());

  std::swap(*A_ptr, dutchFlagColors);

  return;
}

// O(n^2) time and O(1) space.
void DutchFlagPartition2(int pivot_index, vector<Color> *A_ptr) {
  if (A_ptr->empty())
    return;

  const auto pivot = static_cast<int>(A_ptr->at(pivot_index));

  // Put smaller elements before pivot.
  for (int i = 0; i < A_ptr->size(); ++i) {
    const auto current = static_cast<int>(A_ptr->at(i));
    if (current < pivot)
      continue;

    for (int j = i + 1; j < A_ptr->size(); ++j) {
      if (static_cast<int>(A_ptr->at(j)) < pivot) {
        std::swap(A_ptr->at(i), A_ptr->at(j));
        break;
      }
    }
  }

  // Put bigger elements after pivot.
  for (int i = A_ptr->size() - 1; i >= 0; --i) {
    const auto current = static_cast<int>(A_ptr->at(i));
    if (current > pivot)
      continue;

    for (int j = i - 1; j >= 0; --j) {
      if (static_cast<int>(A_ptr->at(j)) > pivot) {
        std::swap(A_ptr->at(i), A_ptr->at(j));
        break;
      }
    }
  }

  return;
}

// O(n) time and O(1) space.
void DutchFlagPartition3(int pivot_index, vector<Color> *A_ptr) {
  if (A_ptr->empty())
    return;

  const auto pivot = static_cast<int>(A_ptr->at(pivot_index));

  // Make sure all the elements smaller than the pivot come in the beginning.
  auto startIdx = 0;
  auto endIdx = A_ptr->size() - 1;
  while (startIdx < endIdx) {
    const auto currentStartEl = static_cast<int>(A_ptr->at(startIdx));
    if (currentStartEl < pivot)
      ++startIdx;
    else {
      std::swap(A_ptr->at(startIdx), A_ptr->at(endIdx));
      --endIdx;
    }
  }

  if (static_cast<int>(A_ptr->at(startIdx)) < pivot)
    ++startIdx;

  // Now make sure that the pivot elements come before the elements bigger than
  // pivot in the rest of the array.
  endIdx = A_ptr->size() - 1;
  while (startIdx < endIdx) {
    const auto currentStartEl = static_cast<int>(A_ptr->at(startIdx));
    if (currentStartEl == pivot)
      ++startIdx;
    else {
      std::swap(A_ptr->at(startIdx), A_ptr->at(endIdx));
      --endIdx;
    }
  }

  return;
}

// O(n) time and O(1) space.
void DutchFlagPartition4(int pivot_index, vector<Color> *A_ptr) {
  if (A_ptr->empty())
    return;

  const auto pivot = static_cast<int>(A_ptr->at(pivot_index));

  auto smallerIdx = 0;
  for (auto idx = 0; idx < A_ptr->size(); ++idx) {
    const auto currentValue = static_cast<int>(A_ptr->at(idx));

    if (currentValue < pivot) {
      std::swap(A_ptr->at(idx), A_ptr->at(smallerIdx));
      ++smallerIdx;
    }
  }

  auto largerIdx = A_ptr->size() - 1;
  for (int idx = largerIdx; idx >= 0; --idx) {
    const auto currentValue = static_cast<int>(A_ptr->at(idx));

    if (currentValue > pivot) {
      std::swap(A_ptr->at(idx), A_ptr->at(largerIdx));
      --largerIdx;
    }
  }

  return;
}

// O(n) time and O(1) space.
void DutchFlagPartition(int pivot_index, vector<Color> *A_ptr) {
  if (A_ptr->empty())
    return;

  const auto pivot = static_cast<int>(A_ptr->at(pivot_index));

  auto smallerIdx = 0;
  auto largerIdx = A_ptr->size() - 1;

  auto idx = 0;
  while (idx <= largerIdx) {
    const auto currentValue = static_cast<int>(A_ptr->at(idx));

    if (currentValue < pivot) {
      std::swap(A_ptr->at(idx), A_ptr->at(smallerIdx));
      ++smallerIdx;
      ++idx;
    } else if (currentValue > pivot) {
      std::swap(A_ptr->at(idx), A_ptr->at(largerIdx));
      --largerIdx;
    } else
      ++idx;
  }

  return;
}

void DutchFlagPartitionWrapper(TimedExecutor &executor, const vector<int> &A,
                               int pivot_idx) {
  vector<Color> colors;
  colors.resize(A.size());
  std::array<int, 3> count = {0, 0, 0};
  for (size_t i = 0; i < A.size(); i++) {
    count[A[i]]++;
    colors[i] = static_cast<Color>(A[i]);
  }
  Color pivot = colors[pivot_idx];

  executor.Run([&] { DutchFlagPartition(pivot_idx, &colors); });

  int i = 0;
  while (i < colors.size() && colors[i] < pivot) {
    count[static_cast<int>(colors[i])]--;
    ++i;
  }

  while (i < colors.size() && colors[i] == pivot) {
    count[static_cast<int>(colors[i])]--;
    ++i;
  }

  while (i < colors.size() && colors[i] > pivot) {
    count[static_cast<int>(colors[i])]--;
    ++i;
  }

  if (i != colors.size()) {
    throw TestFailure("Not partitioned after " + std::to_string(i) +
                      "th element");
  } else if (count != std::array<int, 3>{0, 0, 0}) {
    throw TestFailure("Some elements are missing from original array");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "A", "pivot_idx"};
  return GenericTestMain(args, "dutch_national_flag.cc",
                         "dutch_national_flag.tsv", &DutchFlagPartitionWrapper,
                         DefaultComparator{}, param_names);
}
