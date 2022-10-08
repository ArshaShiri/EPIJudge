#include <vector>

#include "test_framework/generic_test.h"
using std::vector;

// Time complexity and space complexity are O(ns) where n is final score and s is number of individual scores.
int NumCombinationsForFinalScore(int final_score, const std::vector<int> &individual_play_scores)
{
  if (final_score == 0)
    return 1;

  const auto minIndividualScore = std::min_element(individual_play_scores.begin(), individual_play_scores.end());
  if (*minIndividualScore > final_score)
    return 0;

  std::vector<int> row(final_score + 1, 0);
  std::vector<std::vector<int>> possibleCombinationsForEachScore(individual_play_scores.size(), row);

  // Fill first row.
  auto &firstRow = possibleCombinationsForEachScore.at(0);
  const auto firstRowScore = individual_play_scores.at(0);

  auto colIdx = 0;
  std::for_each(firstRow.begin(), firstRow.end(), [&colIdx, firstRowScore](int &el) {
    el = (colIdx % firstRowScore) == 0 ? 1 : 0;
    colIdx++;
  });

  for (int rowIdx = 1; rowIdx < possibleCombinationsForEachScore.size(); ++rowIdx)
  {
    auto &currentRow = possibleCombinationsForEachScore.at(rowIdx);
    const auto currentIndividualScore = individual_play_scores.at(rowIdx);
    
    for (int colIdx = 0; colIdx < currentRow.size(); colIdx++)
    {
      const auto previousRowValue = possibleCombinationsForEachScore.at(rowIdx - 1).at(colIdx);
      auto previousColumnValue = 0;
      const auto previousColIdx = colIdx - currentIndividualScore;

      if (previousColIdx >= 0)
        previousColumnValue = possibleCombinationsForEachScore.at(rowIdx).at(previousColIdx);

      possibleCombinationsForEachScore.at(rowIdx).at(colIdx) = previousColumnValue + previousRowValue;
    }
  }

  return possibleCombinationsForEachScore.back().back();
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"final_score", "individual_play_scores"};
  return GenericTestMain(args, "number_of_score_combinations.cc",
                         "number_of_score_combinations.tsv",
                         &NumCombinationsForFinalScore, DefaultComparator{},
                         param_names);
}
