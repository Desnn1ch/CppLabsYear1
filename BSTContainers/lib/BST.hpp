#pragma once

#include <memory>
#include <iostream>
#include <string>

namespace Tag {
struct InOrderTag {};
struct PreOrderTag {};
struct PostOrderTag {};
}

template<typename T,
    typename TraversalTag = Tag::PreOrderTag,
    typename Compare = std::less<T>,
    typename Alloc = std::allocator<T>
>
class BinarySearchTree {
 private:
  struct Node;
  struct FakeNode {
    Node* parent;
    Node* left;
    Node* right;

    FakeNode() : parent(nullptr), left(nullptr), right(nullptr) {}
  };
  struct Node: FakeNode {
    T value;

    Node() : FakeNode() {}
    Node(T value) : FakeNode(), value(value) {}
  };
  //iterator
  template<bool IsConstIterator = false>
  class BSTiterator {
   public:
    using value_type = T;
    using reference = std::conditional_t<IsConstIterator, const T&, T&>;
    using pointer = std::conditional_t<IsConstIterator, const Node*, Node*>;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;

    BSTiterator(pointer node) : current_(node) {}
    reference operator*() const {
      return (current_->value);
    }
    pointer operator->() const {
      return current_;
    }
    BSTiterator& operator++() {
      if constexpr (std::is_same<TraversalTag, Tag::InOrderTag>::value) {
        if (current_ == nullptr) {
          return *this;
        }
        if (current_->right != nullptr) {
          current_ = current_->right;
          while (current_->left != nullptr) {
            current_ = current_->left;
          }
        }
        else {
          auto parent = current_->parent;
          while (parent != nullptr && current_ == parent->right) {
            current_ = parent;
            parent = parent->parent;
          }
          current_ = parent;
        }

        return *this;
      }
      else if constexpr (std::is_same<TraversalTag, Tag::PreOrderTag>::value) {
        if (current_ == nullptr) {
          return *this;
        }
        if (current_->left != nullptr) {
          current_ = current_->left;
        } else if (current_->right != nullptr) {
          current_ = current_->right;
        } else {
          pointer parent = current_->parent;
          while (parent != nullptr && (parent->right == nullptr || current_ == parent->right)) {
            current_ = parent;
            parent = parent->parent;
          }
          if (parent != nullptr) {
            current_ = parent->right;
          } else {
            current_ = nullptr;
          }
        }
        return *this;
      }
      else if constexpr (std::is_same<TraversalTag, Tag::PostOrderTag>::value) {
        if (current_->parent == nullptr) {
          current_ = nullptr;
        } else if (current_ == current_->parent->right) {
          current_ = current_->parent;
        } else {
          if (current_->parent->right != nullptr) {
            pointer temp = current_->parent->right;
            while (temp->left != nullptr || temp->right != nullptr) {
              if (temp->left != nullptr) {
                temp = temp->left;
              } else {
                temp = temp->right;
              }
            }
            current_ = temp;
          } else {
            current_ = current_->parent;
          }
        }
        return *this;

      }
    }
    BSTiterator operator++(int) {
      BSTiterator temp(*this);
      ++(*this);
      return temp;
    }
    BSTiterator& operator--() {
      if constexpr (std::is_same<TraversalTag, Tag::InOrderTag>::value) {
        if (current_->left != nullptr) {
          current_ = current_->left;
          while (current_->right != nullptr) {
            current_ = current_->right;
          }
        } else {
          pointer temp1 = current_->parent;
          pointer temp2 = current_;
          while (temp1 != nullptr && temp2 == temp1->left) {
            temp2 = temp1;
            temp1 = temp2->parent;
          }
          current_ = temp1;
        }
        return *this;
      }
      else if constexpr (std::is_same<TraversalTag, Tag::PreOrderTag>::value) {
        if (current_->parent == nullptr) {
          current_ = nullptr;
        } else if (current_->parent->left != nullptr && current_->parent->left != current_ ) {
          pointer temp = current_->parent->left;
          while (temp->right != nullptr || temp->left != nullptr) {
            if (temp->right != nullptr) {
              temp = temp->right;
            } else if (temp->left != nullptr) {
              temp = temp->left;
            }
          }
          current_ = temp;
        } else {
          current_ = current_->parent;
        }
        return *this;
      }
      else if constexpr (std::is_same<TraversalTag, Tag::PostOrderTag>::value)  {
        if (current_->right != nullptr) {
          current_ = current_->right;
        } else if (current_->left != nullptr) {
          current_ = current_->left;
        } else if (current_->parent != nullptr) {
          if (current_->parent->right == current_) {
            if (current_->parent->left != nullptr) {
              current_ = current_->parent->left;
              return *this;
            }
          } else {
            pointer temp = current_->parent;
            if (temp->parent != nullptr && temp->parent->left != nullptr) {
              current_ = temp->parent->left;
              return *this;
            }
          }
          current_ = current_->parent;
        }
        return *this;
      }
    }
    BSTiterator operator--(int) {
      BSTiterator temp(*this);
      --(*this);
      return temp;
    }
    friend bool operator==(const BSTiterator& lhs, const BSTiterator& rhs) {
      return lhs.current_ == rhs.current_;
    }
    friend bool operator!=(const BSTiterator& lhs, const BSTiterator& rhs) {
      return lhs.current_ != rhs.current_;
    }
    bool operator==(const BSTiterator& other)  {
      return current_ == other.current_;
    }
    bool operator!=(const BSTiterator& other)  {
      return current_ != other.current_;
    }

   private:
    pointer current_;
  };

 public:
  using value_type = T;
  using allocator_type = Alloc;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename std::allocator_traits<Alloc>::pointer;
  using const_pointer = typename std::allocator_traits<Alloc>::const_pointer;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using iterator = BSTiterator<false>;
  using const_iterator = BSTiterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  BinarySearchTree(const Alloc& alloc = Alloc())
      : fake_node_(new FakeNode()), size_(), alloc_(alloc), end_(new Node(1)) {};
  BinarySearchTree(const Compare& comp, const Alloc& alloc = Alloc())
      : fake_node_(new FakeNode()), size_(), compare_(comp), alloc_(alloc), end_(new Node(1)) {};
  BinarySearchTree(const BinarySearchTree& other) : fake_node_(new FakeNode()), size_(other.size_),
                                                    compare_(other.compare_), alloc_(other.alloc_), end_(new Node(1)) {
    fake_node_->right = CopyTree(other.fake_node_->right);
    end_ = other.end_;
  }

  template<typename InputIterator>
  BinarySearchTree(InputIterator i, InputIterator j, const Compare& compare = Compare()) : compare_(compare) {
    if constexpr (std::is_same<typename std::iterator_traits<InputIterator>::value_type, T>::value) {
      for (; i != j; ++i) {
        insert(*i);
      }
    }
  }
  BinarySearchTree(std::initializer_list<T> il, const Compare& compare = Compare())
      : BinarySearchTree(il.begin(), il.end(), compare) {}

  bool operator==(const BinarySearchTree& rhs) const {
    return std::equal(cbegin(), cend(), rhs.cbegin(), rhs.cend());
  };
  bool operator!=(const BinarySearchTree& rhs) {
    return !(std::equal(begin(), end(), rhs.begin(), rhs.end()));
  };
  BinarySearchTree& operator=(const BinarySearchTree& other) {
    if (this != &other) {
      DestroyTree(fake_node_->right);
      DeallocateNode(end_);

      fake_node_ = new FakeNode();
      size_ = other.size_;
      compare_ = other.compare_;
      alloc_ = other.alloc_;
      end_ = new Node(1);

      fake_node_->right = CopyTree(other.fake_node_->right);
      fake_node_->left = other.fake_node_->left;
      end_ = other.end_;
    }
    return *this;
  }

  ~BinarySearchTree() {
    DestroyTree(fake_node_->right);
    delete fake_node_;
    delete end_;
  }
  template<typename InputIterator>
  void insert(InputIterator i, InputIterator j) {
    if constexpr (std::is_same<typename std::iterator_traits<InputIterator>::value_type, T>::value) {
      for (; i != j; ++i) {
        insert(*i);
      }
    }
  }
  void insert(std::initializer_list<T> il) {
    insert(il.begin(), il.end());
  }
  void insert(const T& value) {
    if (fake_node_->right == nullptr) {
      fake_node_->right = CreateNode(value, nullptr);
      if (std::is_same<TraversalTag, Tag::InOrderTag>::value) {
        fake_node_->left = fake_node_->right;
      }
      else if (std::is_same<TraversalTag, Tag::PreOrderTag>::value) {
        end_->parent = fake_node_->right;
        fake_node_->right->right = end_;
      } else if (std::is_same<TraversalTag, Tag::PostOrderTag>::value) {
        fake_node_->left = fake_node_->right;
        end_->parent = fake_node_->right;
        fake_node_->right->parent = end_;
      }
    } else {
      fake_node_->right = InsertNode(fake_node_->right, value, nullptr);
    }
    size_++;
  }
  void remove(const T& value) {
    if (fake_node_->right != nullptr) {
      fake_node_->right = RemoveNode(fake_node_->right, value);
      size_--;
    }
  }
  bool find(const T& value) {
    if (Find(fake_node_->right, value)) {
      return true;
    }
    return false;
  };
  bool is_empty() {
    return (size_ == 0);
  }
  size_type size() {
    return size_;
  }
  void clean() {
    DestroyTree(fake_node_->right);
    fake_node_->right = fake_node_->left = nullptr;
    end_->parent = nullptr;
  }

 private:
  Compare compare_;
  FakeNode* fake_node_;
  Node* end_;
  size_t size_;
  typename std::allocator_traits<Alloc>::template rebind_alloc<Node> alloc_;
  Node* InsertNode(Node* root, const T& value, Node* parent) {
    if (root == nullptr) {
      Node* new_node = CreateNode(value, parent);
      RelaxBegin(new_node);
      return new_node;
    }

    if (root == end_) {
      Node* new_node = CreateNode(value, parent);
      parent->right = new_node;
      new_node->right = end_;
      end_->parent = new_node;
      return new_node;
    }

    if (compare_(value, root->value)) {
      root->left = InsertNode(root->left, value, root);
    } else {
      root->right = InsertNode(root->right, value, root);
    }

    return root;
  }
  Node* RemoveNode(Node*  root, const T& value) {
    if (root == nullptr) {
      return nullptr;
    }
    if (compare_(value, root->value)) {
      root->left = RemoveNode(root->left, value);
    } else if (compare_(root->value, value)) {
      root->right = RemoveNode(root->right, value);
    } else {
      if (root->left == nullptr) {
        Node* temp = root->right;
        if (temp != nullptr) {
          temp->parent = root->parent;
        }
        DeallocateNode(root);
        return temp;
      } else if (root->right == nullptr) {
        Node* temp = root->left;
        if (temp != nullptr) {
          temp->parent = root->parent;
        }
        DeallocateNode(root);
        return temp;
      } else {
        Node* min_node = FindMin(root->right);
        root->value = min_node->value;
        root->right = RemoveNode(root->right, min_node->value);
      }
    }
    return root;
  }
  Node* CreateNode(const T& value, Node* parent) {
    Node* new_node = alloc_.allocate(1);
    std::allocator_traits<decltype(alloc_)>::construct(alloc_, new_node, Node());
    new_node->value = value;
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = parent;
    return new_node;
  }
  void DeallocateNode(Node* node) {
    std::allocator_traits<decltype(alloc_)>::destroy(alloc_, node);
    std::allocator_traits<decltype(alloc_)>::deallocate(alloc_, node, 1);
  }
  void RelaxBegin(Node* node) {
    if (std::is_same<TraversalTag, Tag::InOrderTag>::value || std::is_same<TraversalTag, Tag::PostOrderTag>::value) {
      if (fake_node_->left->left != nullptr) {
        fake_node_->left = fake_node_->left->left;
      }
    }
  }
  Node* Find (Node* parent, const T& value) {
    if (parent == nullptr || parent->value == value) {
      return parent;
    }
    if (value < parent->value) {
      return Find(parent->left, value);
    } else if (value > parent->value) {
      return Find(parent->right, value);
    }
    return parent;
  }
  Node* FindMin(Node* root) {
    if (root == nullptr)
      return nullptr;
    while (root->left != nullptr)
      root = root->left;
    return root;
  }
  Node* FindMax(Node* root) {
    if (root == nullptr)
      return nullptr;
    while (root->right != nullptr)
      root = root->right;
    return root;
  }
  Node* CopyTree(Node* root) {
    if (root == nullptr) {
      return nullptr;
    }
    Node* new_node = CreateNode(root->value, nullptr);
    new_node->left = CopyTree(root->left);
    new_node->right = CopyTree(root->right);

    return new_node;
  }
  void DestroyTree(Node* root) {
    if (root != nullptr) {
      DestroyTree(root->left);
      DestroyTree(root->right);
      DeallocateNode(root);
    }
  }

 public:
  iterator begin() const {
    if constexpr (std::is_same<TraversalTag, Tag::InOrderTag>::value) {
      return iterator(fake_node_->left);
    } else if constexpr (std::is_same<TraversalTag, Tag::PreOrderTag>::value) {
      return iterator(fake_node_->right);
    } else if constexpr (std::is_same<TraversalTag, Tag::PostOrderTag>::value) {
      return iterator(fake_node_->left);
    }
  }
  iterator end() const {
    return iterator(end_);
  }
  const_iterator cbegin() const {
    if constexpr (std::is_same<TraversalTag, Tag::InOrderTag>::value) {
      return const_iterator(fake_node_->left);
    } else if constexpr (std::is_same<TraversalTag, Tag::PreOrderTag>::value) {
      return const_iterator(fake_node_->right);
    } else if constexpr (std::is_same<TraversalTag, Tag::PostOrderTag>::value) {
      return const_iterator(fake_node_->left);
    }
  }
  const_iterator cend() const {
    return const_iterator(end_);
  }
  reverse_iterator rbegin() const {
    return reverse_iterator(end());

  }
  reverse_iterator rend() const {
    if constexpr (std::is_same<TraversalTag, Tag::InOrderTag>::value) {
      return reverse_iterator(fake_node_->left);
    } else if constexpr (std::is_same<TraversalTag, Tag::PreOrderTag>::value) {
      return reverse_iterator(fake_node_->right);
    } else if constexpr (std::is_same<TraversalTag, Tag::PostOrderTag>::value) {
      return reverse_iterator(fake_node_->left);
    }

  }
  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(end_);
  }
  const_reverse_iterator crend() const {
    return const_reverse_iterator(rend());
  }
};
