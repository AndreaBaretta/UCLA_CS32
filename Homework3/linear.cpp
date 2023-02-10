#include <cmath>
#include <cassert>
#include <iostream>

bool somePredicate(double x) {
    // return std::sin(std::exp(x)) >
    //        std::cos(x);  // include <cmath> for std::sin, etc.
    return x >= 10;
}

// Return true if the somePredicate function returns true for at
// least one of the array elements; return false otherwise.
bool anyTrue(const double a[], int n) {
    if (n <= 0) { return false; }
    if (somePredicate(a[n-1])) {
        return true;
    } else {
        return anyTrue(a, n-1);
    }
}

// Return the number of elements in the array for which the
// somePredicate function returns true.
int countTrue(const double a[], int n) {
    if (n <= 0) { return 0; }
    if (somePredicate(a[n-1])) {
        return 1 + countTrue(a, n-1);
    } else {
        return countTrue(a, n-1);
    }
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n) {
    if (n <= 0) { return -1; }
    int recursion = firstTrue(a, n-1);
    if (somePredicate(a[n-1]) && recursion == -1) {
        return n-1;
    } else {
        return recursion;
    }
}

// Return the subscript of the smallest element in the array (i.e.,
// return the smallest subscript m such that a[m] <= a[k] for all
// k from 0 to n-1).  If the function is told that no doubles are to
// be considered in the array, return -1.
int indexOfMinimum(const double a[], int n) {
    if (n <= 0) { return -1; }
    double cur = a[n-1];
    int prev_index = indexOfMinimum(a, n-1);
    double prev = a[prev_index];
    if (prev_index == -1) {
        return n-1;
    } else if (prev <= cur) {
        return prev_index;
    } else {
        return n-1;
    }
}

// If all n1 elements of a1 appear in the n2 element array a2, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 is not a
// not-necessarily-contiguous subsequence of a2), return false.
// (Of course, if a1 is empty (i.e., n1 is 0), return true.)
// For example, if a2 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a1 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a1 is
//    50 30 20
// or
//    10 20 20
bool isIn(const double a1[], int n1, const double a2[], int n2) {
    if (n1 == 0) {
        return true;
    }

    if (n1 > n2 || n2 == 0) {
        return false;
    }

    if (a1[n1-1] == a2[n2-1]) {
        return isIn(a1, n1-1, a2, n2-1);
    } else {
        return isIn(a1, n1, a2, n2-1);
    }
}

int main() {
    {
        double x[] = {1,3,5,6,10,3,5};
        assert(anyTrue(x, 7));
        assert(anyTrue(x, 5));
        assert(!anyTrue(x, 4));
        assert(!anyTrue(x, 1));
        assert(!anyTrue(x, 0));
    }
    {
        double x[] = {10,3,5,6,10,3,15,3,5};
        assert(countTrue(x, 9) == 3);
        assert(countTrue(x, 7) == 3);
        assert(countTrue(x, 6) == 2);
        assert(countTrue(x, 5) == 2);
        assert(countTrue(x, 4) == 1);
        assert(countTrue(x, 1) == 1);
        assert(countTrue(x, 0) == 0);
    }
    {
        double x[] = {1,10,3,5,6,10,3,15,3,5};
        assert(firstTrue(x, 10) == 1);
        assert(firstTrue(x, 8) == 1);
        assert(firstTrue(x, 7) == 1);
        assert(firstTrue(x, 6) == 1);
        assert(firstTrue(x, 5) == 1);
        assert(firstTrue(x, 2) == 1);
        assert(firstTrue(x, 1) == -1);
        assert(firstTrue(x, 0) == -1);
    }
    {
        double x[] = {4,10,7,5,3,10,3,15,1,5};
        assert(indexOfMinimum(x, 10) == 8);
        assert(indexOfMinimum(x, 8) == 4);
        assert(indexOfMinimum(x, 7) == 4);
        assert(indexOfMinimum(x, 6) == 4);
        assert(indexOfMinimum(x, 5) == 4);
        assert(indexOfMinimum(x, 4) == 0);
        assert(indexOfMinimum(x, 2) == 0);
        assert(indexOfMinimum(x, 1) == 0);
        assert(indexOfMinimum(x, 0) == -1);
    }
    {
        double a2[] = {10,50,40,20,50,40,30};
        double a1_1[] = {50,20,30};
        double a1_2[] = {50,40,40};
        double a1_3[] = {50,30,20};
        double a1_4[] = {10,20,20};
        assert(isIn(a1_1, 3, a2, 7));
        assert(isIn(a1_2, 3, a2, 7));
        assert(!isIn(a1_3, 3, a2, 7));
        assert(!isIn(a1_4, 3, a2, 7));
    }
    std::cout << "Tests passed!" << std::endl;
}