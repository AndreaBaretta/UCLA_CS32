#include <concepts>
#include <iostream>
#include <string>
#include <utility>
#include <stack>

using namespace std;

class Tree {
 public:
  struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int data) : data(data), left(nullptr), right(nullptr) {}
    bool operator<(const Node& node) {
      return data < node.data;
    }
  };

  Node* head;
  Tree() : head(nullptr) {}

  void insert(int data) {
    if (head == nullptr) {
      head = new Node(data);
    } else {
      insert(data, head);
    }
  }

  void remove(int data) {
    if (head == nullptr) {
      return;
    }
    head = remove(data, head);
  }

  void print() { print(head, ""); }

  template <same_as<int>... ints>
  void insert(ints... data) {
    ((void)insert(std::forward<ints>(data)), ...);
  }

  template <same_as<int>... ints>
  void remove(ints... data) {
    ((void)remove(std::forward<ints>(data)), ...);
  }

  void printPreorder() {
    printPreorder(head);
    cout << endl;
  }

  void printInorder() {
    printInorder(head);
    cout << endl;
  }

  void printPostorder() {
    printPostorder(head);
    cout << endl;
  }

  ~Tree() { free(head); }

 private:
  void insert(int data, Node* node) {
    if (data < node->data) {
      if (node->left == nullptr) {
        node->left = new Node(data);
      } else {
        insert(data, node->left);
      }
    } else if (data > node->data) {
      if (node->right == nullptr) {
        node->right = new Node(data);
      } else {
        insert(data, node->right);
      }
    }  // Else? Meh smeh
  }

  Node* remove(int data, Node* node) {
    if (node == nullptr) {
      return nullptr;
    }
    if (data < node->data) {
      node->left = remove(data, node->left);
      return node;
    } else if (data > node->data) {
      node->right = remove(data, node->right);
      return node;
    } else {
      if (node->left == nullptr && node->right == nullptr) {
        delete node;
        return nullptr;
      } else if (node->left == nullptr) {
        Node* tmp = node->right;
        delete node;
        return tmp;
      } else if (node->right == nullptr) {
        Node* tmp = node->left;
        delete node;
        return tmp;
      } else {
        // cout << "sup" << endl;
        int replacement_data = findGreatest(node->left)->data;
        // cout << "Chosen replacement for " << data << ": " << replacement_data << endl;
        node->data = replacement_data;
        node->left = remove(replacement_data, node->left);
        return node;
      }
    }
  }

  void print(Node* n, string s) {
    if (n == nullptr) {
      cout << s << "XX" << endl;
    } else {
      cout << s << n->data << endl;
      if (n->left == nullptr && n->right == nullptr) {
        return;
      }
      print(n->left, s + "    ");
      print(n->right, s + "    ");
    }
  }

  void printPreorder(Node* n) {
    if (n == nullptr) {
      return;
    }
    cout << n->data << " ";
    printPreorder(n->left);
    printPreorder(n->right);
  }

  void printInorder(Node* n) {
    if (n == nullptr) {
      return;
    }
    printInorder(n->left);
    cout << n->data << " ";
    printInorder(n->right);
  }

  void printPostorder(Node* n) {
    if (n == nullptr) {
      return;
    }
    printPostorder(n->left);
    printPostorder(n->right);
    cout << n->data << " ";
  }

  void free(Node* n) {
    if (n != nullptr) {
      free(n->left);
      free(n->right);
      delete n;
    }
  }

  Node* findGreatest(Node* node) {
    if (node == nullptr) {
      return nullptr;
    }
    Node* l = findGreatest(node->left);
    Node* r = findGreatest(node->right);
    return max(max(node, l), r);
  }

  Node* max(Node* a, Node* b) {
    if (a == nullptr) {
      return b;
    }
    if (b == nullptr) {
      return a;
    }
    return (a->data < b->data) ? b : a;
  }

};

int main() {
  Tree tree;
  tree.insert(50, 60, 70, 20, 10, 40, 30);
  // tree.print();
  tree.insert(80, 62, 75, 15, 35, 23);
  // tree.print();
  // tree.printPreorder();
  // tree.printInorder();
  // tree.printPostorder();
  tree.print();
  cout << "------------------" << endl;
  tree.remove(30, 20);
  tree.print();
}