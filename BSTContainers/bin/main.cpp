#include <iostream>
#include <memory>

#include <lib/BST.hpp>

#include "set"

int main(int argc, char** argv) {
  BinarySearchTree<int, Tag::PreOrderTag> tree;
  tree.insert(10);
  tree.insert(12);
  tree.insert(15);
  tree.insert(9);
  tree.insert(4);
  tree.insert(5);
  tree.insert(11);
  tree.insert(2);
  tree.insert(20);
  for (auto el : tree) {
    std::cout << el << " ";
  } std::cout << '\n';
  

  return 0;
}
