#include "list_node.h"
#include "test_framework/generic_test.h"


// O(n) time and O(n) space since a new list is generated.
shared_ptr<ListNode<int>> MergeTwoSortedLists1(shared_ptr<ListNode<int>> l1,
                                              shared_ptr<ListNode<int>> l2) {
  if ((l1.use_count() == 0) && (l2.use_count() == 0))
    return nullptr;

  if (l1.use_count() == 0)
    return l2;

  if (l2.use_count() == 0)
    return l1;

  std::shared_ptr<ListNode<int>> head;

  if (l1->data < l2->data) {
    head = std::make_shared<ListNode<int>>(l1->data);
    l1 = l1->next;
  } else {
    head = std::make_shared<ListNode<int>>(l2->data);
    l2 = l2->next;
  }

  ListNode<int> *tail = head.get();

  auto l1IsValid = false;
  auto l2IsValid = false;

  do {
    l1IsValid = l1 != nullptr;
    l2IsValid = l2 != nullptr;

    if (l1IsValid && l2IsValid) {
      const auto l1Data = l1->data;
      const auto l2Data = l2->data;

      if (l1Data < l2Data) {
        tail->next = std::make_shared<ListNode<int>>(l1Data);
        l1 = l1->next;
      } else {
        tail->next = std::make_shared<ListNode<int>>(l2Data);
        l2 = l2->next;
      }
    } else if (l1IsValid) {
      tail->next = std::make_shared<ListNode<int>>(l1->data);
      l1 = l1->next;
    } else if (l2IsValid) {
      tail->next = std::make_shared<ListNode<int>>(l2->data);
      l2 = l2->next;
    } else {
      break;
    }

    tail = tail->next.get();

  } while (l1IsValid || l2IsValid);

  return head;
}

// O(n) time and O(1) space since it modifies the given lists and does not generate a new one.
std::shared_ptr<ListNode<int>> MergeTwoSortedLists(std::shared_ptr<ListNode<int>> l1,
                                                    std::shared_ptr<ListNode<int>> l2)
{
  if ((l1.use_count() == 0) && (l2.use_count() == 0))
    return nullptr;

  auto dummyHead = std::make_shared<ListNode<int>>();
  auto tail = dummyHead;


  while ((l1 != nullptr) && (l2 != nullptr))
  {
    const auto l1Data = l1->data;
    const auto l2Data = l2->data;

    if (l1Data < l2Data)
    {
      tail->next = l1;
      l1 = l1->next;
    }
    else
    {
      tail->next = l2;
      l2 = l2->next;
    }

    tail = tail->next;
  }

  // Append the rest of the nodes.
  tail->next = (l1.use_count() != 0) ? l1 : l2;

  return dummyHead->next;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L1", "L2"};
  return GenericTestMain(args, "sorted_lists_merge.cc",
                         "sorted_lists_merge.tsv", &MergeTwoSortedLists,
                         DefaultComparator{}, param_names);
}
