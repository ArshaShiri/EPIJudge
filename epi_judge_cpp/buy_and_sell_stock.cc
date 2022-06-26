#include <iostream>
#include <vector>

#include "test_framework/generic_test.h"
using std::vector;
double BuyAndSellStockOnce(const vector<double> &prices) {
  auto maxProfit = 0.0;

  for (auto buyPriceIdx = 0; buyPriceIdx < prices.size(); ++buyPriceIdx) {
    const auto buyPrice = prices.at(buyPriceIdx);

    for (auto sellPriceIdx = buyPriceIdx + 1; sellPriceIdx < prices.size();
         ++sellPriceIdx) {
      const auto sellPrice = prices.at(sellPriceIdx);
      const auto profit = sellPrice - buyPrice;

      if (profit > maxProfit)
        maxProfit = profit;
    }
  }

  return maxProfit;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"prices"};
  return GenericTestMain(args, "buy_and_sell_stock.cc",
                         "buy_and_sell_stock.tsv", &BuyAndSellStockOnce,
                         DefaultComparator{}, param_names);
}
