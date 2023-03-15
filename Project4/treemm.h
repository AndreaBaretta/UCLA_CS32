#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <iostream>
#include <list>

/**
 * Citation: The following implementation of red black tree is based on the data
 * structure by Pf. Robert Sedgewick presented in his article "Left-leaning
 * Red-Black Trees."
 * The article can be found here:
 * https://sedgewick.io/wp-content/themes/sedgewick/papers/2008LLRB.pdf
 */
template <typename KeyType, typename ValueType>
class TreeMultimap {
 private:
  static constexpr const bool RED = true;
  static constexpr const bool BLACK = false;
  struct RBNode {
    const KeyType key;
    std::list<ValueType> values;
    bool color;
    RBNode* right;
    RBNode* left;
    RBNode(const KeyType k, const ValueType v)
        : key(k), values{v}, color(RED), right(nullptr), left(nullptr) {}
  };
  RBNode* root;

 public:
  class Iterator {
   private:
    RBNode* p;
    typename std::list<ValueType>::iterator it;

   public:
    Iterator(RBNode* p) : p(p) {
      if (p != nullptr) {
        it = p->values.begin();
      }
      // it = p->values.begin();
    }

    ValueType& get_value() const { return *it; }

    bool is_valid() const {
      if (p == nullptr) {
        return false;
      }
      return it != p->values.end();
    }

    void advance() {
      // Replace this line with correct code.
      if (is_valid()) {
        ++it;
      }
    }
  };

  TreeMultimap() : root(nullptr) {}

  ~TreeMultimap() { free(root); }

  void insert(const KeyType& key, const ValueType& value) {
    root = insert(key, value, root);
    root->color = BLACK;
  }

  Iterator find(const KeyType& key) const { return find(key, root); }

 private:
  void free(RBNode* p) {
    if (p == nullptr) {
      return;
    }
    free(p->left);
    free(p->right);
    delete p;
  }

  Iterator find(const KeyType& key, RBNode* p) const {
    if (p == nullptr) {
      return Iterator(p);
    } else if (p->key < key) {
      return find(key, p->right);
    } else if (p->key > key) {
      return find(key, p->left);
    }
    return Iterator(p);
  }

  RBNode* rotateLeft(RBNode* p) {
    RBNode* x = p->right;
    p->right = x->left;
    x->left = p;
    x->color = p->color;
    p->color = RED;
    return x;
  }

  RBNode* rotateRight(RBNode* p) {
    RBNode* x = p->left;
    p->left = x->right;
    x->right = p;
    x->color = p->color;
    p->color = RED;
    return x;
  }

  void colorFlip(RBNode* p) {
    p->color = !p->color;
    p->left->color = !p->left->color;
    p->right->color = !p->right->color;
  }

  bool isRed(RBNode* p) {
    if (p == nullptr) {
      return false;
    }
    return p->color;
  }

  RBNode* insert(const KeyType& key, const ValueType& value, RBNode* p) {
    if (p == nullptr) {
      return new RBNode(key, value);
    }
    if (isRed(p->left) && isRed(p->right)) {
      colorFlip(p);
    }
    if (key == p->key) {
      p->values.insert(p->values.end(), value);
    } else if (key < p->key) {
      p->left = insert(key, value, p->left);
    } else {
      p->right = insert(key, value, p->right);
    }
    if (isRed(p->right) && !isRed(p->left)) {
      p = rotateLeft(p);
    }
    if (isRed(p->left) && isRed(p->left->left)) {
      p = rotateRight(p);
    }
    return p;
  }

  void print(RBNode* p, int dist) {  // TODO: Maybe delete this
    if (p == nullptr) {
      return;
    }
    std::cout << p->key << " (" << dist << ")" << std::endl;
    print(p->left, dist + 1);
    print(p->right, dist + 1);
  }

  void printBlackDist(RBNode* p, int dist) {  // TODO: Maybe delete this
    if (p == nullptr) {
      std::cout << dist << ",";
    } else {
      if (!isRed(p)) {
        dist++;
      }
      printBlackDist(p->left, dist);
      printBlackDist(p->right, dist);
    }
  }

 public:          // TODO: Maybe delete this
  void print() {  // TODO: Maybe delete this
    print(root, 0);
  }

  void printBlackDist() {  // TODO: Maybe delete this
    printBlackDist(root, 1);
    std::cout << std::endl;
  }
};

#endif  // TREEMULTIMAP_INCLUDED
