#include <string>

#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
using std::string;

// O(n) time and O(1) space
std::string IntToString(int x) {
  const auto isNegative = x < 0 ? true : false;

  if (isNegative)
    x = -x;

  std::string result;
  const auto zeroChar = '0';

  do {
    result.push_back(zeroChar + (x % 10));
    x = x / 10;
  } while (x != 0);

  if (isNegative)
    result.push_back('-');

  return {result.rbegin(), result.rend()};
}

// O(n) time and O(1) space
int StringToInt1(const std::string &s) {
  if (s.empty())
    return 0;

  auto result = 0;
  auto base = 1;

  const auto zeroChar = '0';

  for (auto rIt = s.rbegin(); rIt != s.rend(); ++rIt) {
    const auto currentValue = *rIt;
    if (currentValue == '-') {
      result = -result;
      break;
    }

    if (currentValue == '+') {
      break;
    }

    result = result + ((currentValue - zeroChar) * base);
    base *= 10;
  }

  return result;
}

int StringToInt(const std::string &s) {
  return (s.at(0) == '-' ? -1 : 1) *
         std::accumulate(
             s.begin() + ((s.at(0) == '-') || (s.at(0) == '+') ? 1 : 0),
             s.end(), 0, [](int sum, char c) { return sum * 10 + c - '0'; });
}

void Wrapper(int x, const string &s) {
  if (stoi(IntToString(x)) != x) {
    throw TestFailure("Int to string conversion failed");
  }

  if (StringToInt(s) != x) {
    throw TestFailure("String to int conversion failed");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "s"};
  return GenericTestMain(args, "string_integer_interconversion.cc",
                         "string_integer_interconversion.tsv", &Wrapper,
                         DefaultComparator{}, param_names);
}
