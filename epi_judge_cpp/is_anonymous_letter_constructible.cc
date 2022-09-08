#include <string>
#include <unordered_map>

#include "test_framework/generic_test.h"
using std::string;
using std::unordered_map;

bool IsLetterConstructibleFromMagazine1(const string &letter_text,
                                        const string &magazine_text) {
  if (letter_text.empty())
    return true;

  if (magazine_text.empty())
    return false;

  std::unordered_map<char, size_t> letterCharactersCount;
  for (const auto chracter : letter_text) {
    const auto [el, isInserted] =
        letterCharactersCount.emplace(std::make_pair(chracter, 0));
    if (!isInserted)
      el->second++;
  }

  std::unordered_map<char, size_t> magazineCommonCharactersCount;
  for (const auto chracter : magazine_text) {
    if (!letterCharactersCount.contains(chracter))
      continue;

    const auto [el, isInserted] =
        magazineCommonCharactersCount.emplace(std::make_pair(chracter, 0));
    if (!isInserted)
      el->second++;
  }

  for (const auto [chracter, count] : letterCharactersCount) {
    if (!magazineCommonCharactersCount.contains(chracter))
      return false;

    const auto countIt = magazineCommonCharactersCount.find(chracter);

    if (count > countIt->second)
      return false;
  }

  return true;
}

bool IsLetterConstructibleFromMagazine2(const string &letter_text,
                                        const string &magazine_text) {
  unordered_map<char, int> char_frequency_for_letter;
  // Compute the frequencies for all chars in letter_text.
  for (char c : letter_text) {
    ++char_frequency_for_letter[c];
  }

  // Check if the characters in magazine_text can cover characters
  // in char_frequency_for_letter.
  for (char c : magazine_text) {
    if (auto it = char_frequency_for_letter.find(c);
        it != cend(char_frequency_for_letter)) {
      --it->second;
      if (it->second == 0) {
        char_frequency_for_letter.erase(it);
        if (empty(char_frequency_for_letter)) {
          // All characters for letter_text are matched.
          break;
        }
      }
    }
  }
  // Empty char_frequency_for_letter means every char in letter_text can be
  // covered by a character in magazine_text.
  return empty(char_frequency_for_letter);
}

bool IsLetterConstructibleFromMagazine(const std::string &letter_text,
                                       const std::string &magazine_text) {
  std::unordered_map<char, size_t> letterCharactersCount;

  for (const auto chracter : letter_text) {
    const auto [el, isInserted] =
        letterCharactersCount.emplace(std::make_pair(chracter, 1));
    if (!isInserted)
      el->second++;
  }

  for (const auto chracter : magazine_text) {
    if (const auto countIt = letterCharactersCount.find(chracter);
        countIt != letterCharactersCount.end()) {
      countIt->second--;
      if (countIt->second == 0)
        letterCharactersCount.erase(countIt);

      if (letterCharactersCount.empty())
        return true;
    }
  }

  for (const auto [chracter, count] : letterCharactersCount) {
    if (count != 0)
      return false;
  }

  return true;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"letter_text", "magazine_text"};
  return GenericTestMain(args, "is_anonymous_letter_constructible.cc",
                         "is_anonymous_letter_constructible.tsv",
                         &IsLetterConstructibleFromMagazine,
                         DefaultComparator{}, param_names);
}
