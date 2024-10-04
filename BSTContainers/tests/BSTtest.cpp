#include <gtest/gtest.h>
#include <lib/BST.hpp>
#include <sstream>

TEST(BSTtest, Insert) {
  BinarySearchTree<int, Tag::PreOrderTag> tree;
  tree.insert(10);
  tree.insert(15);
  tree.insert(5);
  ASSERT_EQ(tree.find(15), true);
}

TEST(BSTtest, RemoveExtreme) {
  BinarySearchTree<int, Tag::PreOrderTag> tree;
  tree.insert(10);
  tree.insert(15);
  tree.insert(5);
  tree.remove(15);
  ASSERT_EQ(tree.find(15), false);
}

TEST(BSTtest, RemoveBase) {
  BinarySearchTree<int, Tag::PreOrderTag> tree;
  tree.insert(10);
  tree.insert(15);
  tree.insert(5);
  tree.remove(10);
  ASSERT_EQ(tree.find(10), false);
}

TEST(BSTtest, TestOperator1) {
  BinarySearchTree<int, Tag::PreOrderTag> tree;
  tree.insert(10);
  tree.insert(15);
  tree.insert(5);
  tree.insert(9);
  tree.insert(2);
  BinarySearchTree<int, Tag::PreOrderTag> tree2;
  tree2 = tree;
  ASSERT_EQ(tree2.find(5), true);
}

TEST(BSTtest, TestOperator2) {
  BinarySearchTree<int, Tag::PreOrderTag> tree;
  tree.insert(10);
  tree.insert(15);
  tree.insert(2);

  ASSERT_EQ(*++tree.begin(), 2);
}

TEST(BSTtest, TestOperator3) {
  BinarySearchTree<int, Tag::PreOrderTag> tree;
  tree.insert(10);
  tree.insert(15);
  tree.insert(2);

  ASSERT_EQ(*--++tree.begin(), *tree.begin());
}

TEST(BSTtest, PreOrder1) {
  BinarySearchTree<int, Tag::PreOrderTag> tree;
  tree.insert(10);
  tree.insert(15);
  tree.insert(5);
  ASSERT_EQ((*tree.begin() + *--tree.end()), 25);
}

TEST(BSTtest, PreOrder2) {
  BinarySearchTree<int, Tag::PreOrderTag> tree;
  tree.insert(10);
  tree.insert(12);
  tree.insert(9);
  tree.insert(2);
  tree.insert(5);
  bool is_ok = false;
  auto it = tree.begin();
  if (*it == 10 && *++it == 9 && *++it == 2 && *++it == 5 && *++it == 12) {
    is_ok = true;
  }
  ASSERT_EQ(is_ok, true);
}