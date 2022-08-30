#include "binary_tree_node.h"
#include "test_framework/generic_test.h"

using IsBalancedAndHeight = std::pair<bool, int>;

IsBalancedAndHeight checkIfBalanced(const BinaryTreeNode<int> *tree) {
  if (tree == nullptr)
    return {true, -1};

  const auto leftSide = checkIfBalanced(tree->left.get());
  if (!leftSide.first)
    return {false, 0};

  const auto rightSide = checkIfBalanced(tree->right.get());
  if (!rightSide.first)
    return {false, 0};

  if (abs(leftSide.second - rightSide.second) > 1)
    return {false, 0};

  const auto size = std::max(leftSide.second, rightSide.second) + 1;
  return {true, size};
}

bool IsBalanced(const unique_ptr<BinaryTreeNode<int>> &tree) {
  if (tree == nullptr)
    return true;

  if ((tree->left == nullptr) && (tree->right == nullptr))
    return true;

  return checkIfBalanced(tree.get()).first;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "is_tree_balanced.cc", "is_tree_balanced.tsv",
                         &IsBalanced, DefaultComparator{}, param_names);
}
