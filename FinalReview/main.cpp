#include <iostream>
#include "sorts.hpp"

int main() {
  std::vector<int> a = {5, 3, 7, 6, 1, 8, 2, 4};
  // selectionSort(a);
  // bubbleSort(a);
  // rotateRight(&a[0], a.size());
  // insertionSort(a);
  // mergeSort(a, 0, a.size()-1);
  // quickSort(a, 0, 7);
  // for (int i = 0; i < 10; ++i) {
  //   heapify(a, 0);
  //   print_container(a);
  // }
  heapSort(a);
  print_container(a);

  // Merge sorting: l=2 m=2 r=3 - 3 5 7 6 1 8 2 4
  // std::vector<int> b = {3,5,7,6,1,8,2,4};
  // merge(b, 2,2,3);
  // print(b);
  // int x;
  // int y;
  // partition(a, 0,7,4,x,y);
  // print(a);
}