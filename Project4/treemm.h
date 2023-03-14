#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <list>

template <typename KeyType, typename ValueType>
class TreeMultimap {
 private:
  constexpr const bool RED = true;
  constexpr const bool BLACK = false;
  struct RBNode {
    const KeyType key;
    std::list<ValueType> values;
    bool color;
    RBNode* right;
    RBNode* left;
    RBNode(const KeyType& k, const ValueType v)
        : key(k), values{v}, color(RED), right(nullptr), left(nullptr) {}
  };
  RBNode* root;

 public:
  class Iterator {
   private:
    std::list<ValueType>::iterator it;

   public:
    Iterator(RBNode* p) {
      // Replace this line with correct code.
      it = p->values.begin();

    }

    ValueType& get_value() const {
      return *it;  // Replace this line with correct code.
    }

    bool is_valid() const {
      it->;  // Replace this line with correct code.
    }

    void advance() {
      // Replace this line with correct code.
    }

   private:
  };

  TreeMultimap() : RBNode(nullptr) {}

  ~TreeMultimap() { free(root); }

  void insert(const KeyType& key, const ValueType& value) {
    insert(key, value, root);
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
    if (p->key < key) {
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
    x.color = p.color;
    p.color = RED;
    return x;
  }

  RBNode* rotateRight(RBNode* p) {
    RBNode* x = p->left;
    p->left = x->right;
    x->right = p;
    x.color = p.color;
    p.color = RED;
    return x;
  }

  void colorFlip(RBNode* p) {
    p->color = !p->color;
    p->left->color = !p->left->color;
    p->right->color =!p->right->color;
  }

  bool isRed(RBNode* p) {
    if (p == nullptr) { return false; }
    return p->color;
  }

  RBNode* insert(const KeyType& key, const ValueType& value, const RBNode* p) {
    if (p == nullptr) {
      new RBNode(key, value);
    }
    if (isRed(p.left) && isRed(p.right)) {
      colorFlip(p);
    }
    int cmp = key.compareTo(p.key);
    if (key == p.key) {
      p->values->insert(p.end(), value);
    } else if (key < h.key) {
      p.left = insert(p.left, key, value);
    } else {
      p.right = insert(p.right, key, value);
    }
    if (isRed(p.right) && !isRed(p.left)) {
      p = rotateLeft(p);
    }
    if (isRed(p.left) && isRed(p.left.left)) {
      p = rotateRight(p);
    }
    return p;
  }
};

#endif  // TREEMULTIMAP_INCLUDED
