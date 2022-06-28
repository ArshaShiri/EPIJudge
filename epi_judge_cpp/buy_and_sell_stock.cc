#include <iostream>
#include <vector>

#include "test_framework/generic_test.h"
using std::vector;
double BuyAndSellStockOnce1(const vector<double> &prices) {
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

// O(n) time and O(n) space
double BuyAndSellStockOnce2(const std::vector<double> &prices) {
  std::vector<double> maxPrices(prices.size());

  // Generate a max price list, a price at index i shows the max price between
  // price.at(i) to price.at(size - 1) This way we don't have to iterate the
  // price list again to check the max price for selling.
  double previousMaxPrice = 0.0;
  auto priceIdx = prices.size() - 1;
  for (auto rIt = maxPrices.rbegin(); rIt != maxPrices.rend(); ++rIt) {
    previousMaxPrice = std::max(prices.at(priceIdx), previousMaxPrice);
    *rIt = previousMaxPrice;
    priceIdx--;
  }

  auto maxProfit = 0.0;
  for (auto idx = 0; idx < static_cast<int>(prices.size()) - 1; ++idx) {
    const auto profit = maxPrices.at(idx + 1) - prices.at(idx);
    maxProfit = std::max(maxProfit, profit);
  }

  return maxProfit;
}

// O(n) time and O(1) space
double BuyAndSellStockOnce(const std::vector<double> &prices)
{
  auto maxProfit = 0.0;
  auto minPriceSoFar = std::numeric_limits<double>::max();

  for (auto price : prices)
  {
    minPriceSoFar = std::min(price, minPriceSoFar);
    const auto profit = price - minPriceSoFar;
    maxProfit = std::max(maxProfit, profit);
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
