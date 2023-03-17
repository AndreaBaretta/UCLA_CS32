#include <algorithm>
#include <deque>
#include <iostream>
#include <string>
#include<cstdlib>

using namespace std;

class Heap {
 private:
  deque<int> data;

 public:
  Heap() : data() {}

  void insert(int val) {
    data.push_back(val);

    int pos = data.size()-1;
    while (pos != 0) {
      if (data[parent(pos)] < data[pos]) {
        swap(data[parent(pos)], data[pos]);
        pos = parent(pos);
      } else {
        break;
      }
    }
  }

  template <same_as<int>... ints>
  void insert(ints... data) {
    ((void)insert(std::forward<ints>(data)), ...);
  }

  void print() { print(0, ""); }

  void printArray() {
    for (int x : data) {
      cout << x << " ";;
    }
    cout << endl;
  }

  int pop() {
    if (data.size() == 0) {
      return 0;
    }
    int ret = data[0];
    swap(data[0], data[data.size()-1]);
    data.pop_back();
    heapifyRebellion(0);
    return ret;
  }

 private:
  int parent(int pos) { return (pos - 1) / 2; }

  int leftChild(int pos) { return (2 * pos) + 1; }

  int rightChild(int pos) { return (2 * pos) + 2; }

  bool isLeaf(int pos) {
    if (pos > (data.size() / 2) && pos <= data.size()) {
      return true;
    }
    return false;
  }

  void print(int pos, string s) {
    if (pos >= data.size()) {
      cout << s << "XX" << endl;
    } else {
      cout << s << data[pos] << endl;
      if (leftChild(pos) >= data.size() && rightChild(pos) >= data.size()) {
        return;
      }
      print(leftChild(pos), s + "    ");
      print(rightChild(pos), s + "    ");
    }
  }

  void heapifyRebellion(int pos) {
    if (isLeaf(pos)) {
      return;
    }
    int leftChild_pos = leftChild(pos);
    if (leftChild_pos >= data.size()) {
      leftChild_pos = pos;
    }
    int rightChild_pos = rightChild(pos);
    if (rightChild_pos >= data.size()) {
      rightChild_pos = pos;
    }

    if (data[pos] < data[leftChild_pos] || data[pos] < data[rightChild_pos]) {
      if (data[leftChild_pos] > data[rightChild_pos]) {
        swap(data[pos], data[leftChild_pos]);
        heapifyRebellion(leftChild_pos);
      } else {
        swap(data[pos], data[rightChild_pos]);
        heapifyRebellion(rightChild_pos);   
      }
    }
  }
};

int main() {
  Heap h;
  // heap.insert(50, 60, 70, 20, 10, 40, 30);
  // heap.insert(80, 62, 75, 15, 35, 23);
  // heap.print();
  // cout << "------------------" << endl;
  // heap.pop();
  // heap.print();
  // cout << "------------------" << endl;
  // heap.pop();
  // heap.print();

  h.insert(5);
	h.insert(6);
	h.insert(0);
	h.insert(4);
	h.insert(10);
	h.insert(3);
	int item;
	item = h.pop();	// Removes the biggest item from the heap, and puts it in item
	h.insert(9);
	h.insert(7);
  item = h.pop();	// Removes the biggest item from the heap, and puts it in item
  h.print();
  cout << "------------------" << endl;
  h.printArray();
  cout << "------------------" << endl;
  item = h.pop();
  h.printArray();

  // srand((unsigned) time(NULL));

	// for (int i = 0; i < 100'000; ++i) {
  //   heap.insert(i);
  // }

}