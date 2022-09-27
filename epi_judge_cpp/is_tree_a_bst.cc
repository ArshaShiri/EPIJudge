#include <optional>
#include <memory>
#include <iostream>
#include <vector>

#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;



// Wrong solution: check test notBSTHeightTwo
bool IsBinaryTreeBSTWrong(const std::unique_ptr<BinaryTreeNode<int>> &tree)
{
  if (tree == nullptr)
    return true;

  const auto isRightBSt = IsBinaryTreeBSTWrong(tree->right);
  const auto isLeftBSt = IsBinaryTreeBSTWrong(tree->left);

  if (!(isRightBSt && isLeftBSt))
    return false;

  const auto getValue = [](const BinaryTreeNode<int> *binTree) {
    return (binTree != nullptr) ? std::optional<int>{ binTree->data } : std::nullopt;
  };

  const auto currentValue = tree->data;
  const auto rightValue = getValue(tree->right.get());
  const auto leftValue = getValue(tree->left.get());


  if ((rightValue != std::nullopt) && (rightValue < currentValue))
    return false;

  if ((leftValue != std::nullopt) && (leftValue > currentValue))
    return false;

  return true;
}

/* *********************************************************************************** */

int getMaxOfSubtree(BinaryTreeNode<int> *tree)
{
  auto max = std::numeric_limits<int>::min();

  if (tree == nullptr)
    return max;

  max = std::max(max, tree->data);
  const auto rightMax = getMaxOfSubtree(tree->right.get());
  const auto leftMax = getMaxOfSubtree(tree->left.get());

  return std::max(max, std::max(rightMax, leftMax));
}

int getMinOfSubtree(BinaryTreeNode<int> *tree)
{
  auto min = std::numeric_limits<int>::max();

  if (tree == nullptr)
    return min;

  min = std::min(min, tree->data);
  const auto rightMin = getMinOfSubtree(tree->right.get());
  const auto leftMin = getMinOfSubtree(tree->left.get());

  return std::min(min, std::min(rightMin, leftMin));
}

// Space complexity is O(h) where h is the height of the tree.
// Time complexity O(n^2) in the worst case where n is the number of elements in the tree.
bool IsBinaryTreeBST1(const std::unique_ptr<BinaryTreeNode<int>> &tree)
{
  if (tree == nullptr)
    return true;

  const auto isRightBST = IsBinaryTreeBST1(tree->right);
  const auto isLeftBST = IsBinaryTreeBST1(tree->left);

  if (!(isRightBST && isLeftBST))
    return false;

  auto leftMax = getMaxOfSubtree(tree->left.get());
  auto rightMin = getMinOfSubtree(tree->right.get());
  const auto value = tree->data;

  if ((leftMax == std::numeric_limits<int>::min()))
    leftMax = value;

  if ((rightMin == std::numeric_limits<int>::max()))
    rightMin = value;

  if ((value < leftMax) || (value > rightMin))
    return false;

  return true;
}

/* *********************************************************************************** */

// Inorder traversal.

// Space complexity is O(h) where h is the height of the tree.
// Time complexity O(n) where n is the number of elements in the tree.
bool isBinaryTreeBSTTraverse(const BinaryTreeNode<int> *tree, int &previousValue)
{
  if (tree == nullptr)
    return true;

  const auto leftIsBST = isBinaryTreeBSTTraverse(tree->left.get(), previousValue);
  if (!leftIsBST)
    return false;

  const auto currentValue = tree->data;
  if (currentValue < previousValue)
    return false;

  previousValue = currentValue;

  const auto rightIsBST = isBinaryTreeBSTTraverse(tree->right.get(), previousValue);
  if (!rightIsBST)
    return false;

  return true;
}

bool IsBinaryTreeBST2(const std::unique_ptr<BinaryTreeNode<int>> &tree)
{
  auto previousValueStart = std::numeric_limits<int>::min();
  return isBinaryTreeBSTTraverse(tree.get(), previousValueStart);
}

/* *********************************************************************************** */

// Space complexity is O(h) where h is the height of the tree.
// Time complexity O(n) where n is the number of elements in the tree.
bool IsBinaryTreeBSTRange3(const BinaryTreeNode<int> *tree, int lowerBound, int upperBound)
{
  if (tree == nullptr)
    return true;

  const auto value = tree->data;
  if ((value < lowerBound) || (value > upperBound))
    return false;

  const auto leftIsBST = IsBinaryTreeBSTRange3(tree->left.get(), lowerBound, value);
  if (!leftIsBST)
    return false;

  const auto rightIsBST = IsBinaryTreeBSTRange3(tree->right.get(), value, upperBound);
  if (!rightIsBST)
    return false;

  return true;
}

bool IsBinaryTreeBST3(const std::unique_ptr<BinaryTreeNode<int>> &tree)
{
  return IsBinaryTreeBSTRange3(tree.get(), std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
}

/* *********************************************************************************** */

struct NodeAndBounds
{
  BinaryTreeNode<int> &node;
  int lowerBound;
  int upperBound;
};

// Space complexity is O(m) where n is the number of elements in the tree.
// Time complexity O(n) where n is the number of elements in the tree. The advantage is that each level is processes
// first before going to the next level. BFS.
bool IsBinaryTreeBST(const std::unique_ptr<BinaryTreeNode<int>> &tree)
{
  if (tree == nullptr)
    return true;

  std::queue<NodeAndBounds> nodesToProcess;

  nodesToProcess.emplace(
    NodeAndBounds{ *(tree.get()), std::numeric_limits<int>::min(), std::numeric_limits<int>::max() });

  while (!nodesToProcess.empty())
  {
    const auto &currentNodeAndBound = nodesToProcess.front();
    const auto currentData = currentNodeAndBound.node.data;

    if (currentData < currentNodeAndBound.lowerBound)
      return false;

    if (currentData > currentNodeAndBound.upperBound)
      return false;

    if (currentNodeAndBound.node.left != nullptr)
      nodesToProcess.emplace(
        NodeAndBounds{ *(currentNodeAndBound.node.left.get()), currentNodeAndBound.lowerBound, currentData });

    if (currentNodeAndBound.node.right != nullptr)
      nodesToProcess.emplace(
        NodeAndBounds{ *(currentNodeAndBound.node.right.get()), currentData, currentNodeAndBound.upperBound });

    nodesToProcess.pop();
  }

  return true;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "is_tree_a_bst.cc", "is_tree_a_bst.tsv",
                         &IsBinaryTreeBST, DefaultComparator{}, param_names);
}
