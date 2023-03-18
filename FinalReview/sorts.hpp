#include <utility>
#include <vector>
#include <iostream>

void print_container(const std::vector<int>& a) {
  for (int i = 0; i < a.size(); ++i) {
    std::cout << a[i] << " ";
  }
  std::cout << std::endl;
}

void rotateRight(int a[], int n) {
  int tmp = a[0];
  for (int i = n-1; i >= 0; --i) {
    a[(i + 1)%n] = a[i];
  }
  a[1] = tmp;
}

void selectionSort(std::vector<int>& a) {
  if (a.size() <= 1) {
    return;
  }
  int sorted = 0;
  while (sorted < a.size()) {
    int max_index = 0;
    for (int i = 0; i < a.size() - sorted; ++i) {
      if (a[i] > a[max_index]) {
        max_index = i;
      }
    }
    std::swap(a[max_index], a[a.size() - sorted - 1]);
    sorted++;
  }
}

void bubbleSort(std::vector<int>& a) {
  if (a.size() <= 1) {
    return;
  }
  bool is_sorted = false;
  int first_ordered_index = a.size()-1;
  while (!is_sorted) {
    int new_sorted_index = 0;
    is_sorted = true;

    for (int i = 0; i < first_ordered_index; ++i) {
      if (a[i] > a[i+1]) {
        std::swap(a[i], a[i+1]);
        is_sorted = false;
        new_sorted_index = i+1;
      }
    }
    first_ordered_index = new_sorted_index;
  }
}

void insertionSort(std::vector<int>& a) {
  if (a.size() <= 1) {
    return;
  }
  int sorted_index = 0;
  while (sorted_index != a.size() - 1) {
    for (int i = 0; i <= sorted_index; ++i) {
      if (a[sorted_index + 1] < a[i]) {
        rotateRight(&a[i], sorted_index + 2 - i);
      }
    }
    ++sorted_index;
  }
}

void merge(std::vector<int>& a, int beginning, int middle, int end) {
  std::vector<int> tmp(a.begin(),a.end());
  int l = beginning;
  int r = middle + 1;
  int i = beginning;
  while (true) {
    if (a[l] < a[r] && l <= middle) {
      tmp[i++] = a[l++];
    } else if (r <= end) {
      tmp[i++] = a[r++];
    } else if (l > middle && r <= end) {
      tmp[i++] = a[r++];
    } else if (r > end && l <= middle) {
      tmp[i++] = a[l++];
    } else {
      std::copy(tmp.begin(), tmp.end(), a.begin());
      return;
    }
  }
}

void mergeSort(std::vector<int>& a, int l, int r) {
  if (l >= r) {
    return;
  }
  int m = l + (r - l)/2;
  mergeSort(a, l, m);
  mergeSort(a, m+1, r);
  merge(a, l, m, r);
}

int partition(std::vector<int>& a, int l, int r, int divider) {
  int firstNotLess = l;
  int firstNotGreater = r;
  int firstUnknown = l;
  while (firstUnknown <= firstNotGreater) {
    if (a[firstUnknown] > divider) { // We've found a number that's greater than the partiton
      std::swap(a[firstUnknown], a[firstNotGreater]); // We don't know the value that was swapped into index firstUnknown
      --firstNotGreater;
    } else if (a[firstUnknown] < divider) { // We've found one that needs to be on the left
      std::swap(a[firstUnknown], a[firstNotLess]);
      ++firstUnknown;
      ++firstNotLess;
    } else { // It's equal to the divider
      ++firstUnknown; // so we know what it is
    }
  }
  return std::max(firstUnknown-1, l);
}

int partition(std::vector<int>& a, int l, int r) {
  int max = std::max(std::max(a[l], a[r]), a[(l+r)/2]);
  int min = std::min(std::min(a[l], a[r]), a[(l+r)/2]);
  int median = a[l] + a[r] + a[(l+r)/2] - max - min; // median
  return partition(a, l, r, median);
}

void quickSort(std::vector<int>& a, int l, int r) {
  if (l >= r) {
    return;
  }
  int pivot = partition(a, l, r);
  quickSort(a, l, pivot);
  quickSort(a, pivot+1, r);
}

int getParent(int x) {
  return (x-1)/2;
}

int leftChild(int x) {
  return 2*x + 1;
}

int rightChild(int x) {
  return 2*x + 2;
}

bool heapify(std::vector<int>& a, int index, int size, bool both_sides = true) {
  if (index >= size) {
    return true;
  }
  bool ret = true;
  int leftChild_index = leftChild(index) < size ? leftChild(index) : index;
  int rightChild_index = rightChild(index) < size ? rightChild(index) : index;

  if (a[index] < a[leftChild_index] || a[index] < a[rightChild_index]) {
    int max_index = leftChild_index;
    if (a[rightChild_index] > a[leftChild_index]) {
      max_index = rightChild_index;
    }
    std::swap(a[index], a[max_index]);
    ret = false;
    if (!both_sides) {
      heapify(a, max_index, size, false);
    }
  }
  if (both_sides) {
    if (leftChild_index != index) {
      ret = ret && heapify(a, leftChild_index, size);
    }
    if (rightChild_index != index) {
      ret = ret && heapify(a, rightChild_index, size);
    }
  }
  return ret;
}

void heapSort(std::vector<int>& a) {
  while (!heapify(a, 0, a.size()));
  int size = a.size();
  // print_container(heap);
  // std::cout << a.size() << std::endl;
  for (int i = a.size()-1; i >= 0; --i) {
    std::swap(a[0], a[size-1]);
    size--;
    heapify(a, 0, size, false);
  }
}