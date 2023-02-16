#include <iostream>
#include <cassert>

// Return the number of ways that all n1 elements of a1 appear in
// the n2 element array a2 in the same order (though not necessarily
// consecutively).  We decree that the empty sequence (i.e. one where
// n1 is 0) appears in a sequence of length n2 in 1 way, even if n2
// is 0.  For example, if a2 is the 7 element array
//	10 50 40 20 50 40 30
// then for this value of a1     the function must return
//	10 20 40                        1
//	10 40 30                        2
//	20 10 40                        0
//	50 40 30                        3
int countIsIn(const double a1[], int n1, const double a2[], int n2) {
    if (n2 < n1 || n1 <= 0 || n2 <= 0) {
        return 0;
    }
    if (a1[n1 - 1] == a2[n2 - 1]) {
        if (n1 == 1) {  // This be the end of de line, matey
            return 1 + countIsIn(a1, n1, a2, n2 - 1);
        } else {  // Else, try for MORE
            return countIsIn(a1, n1 - 1, a2,
                             n2 - 1)  // Using this particular ordering
                   +
                   countIsIn(a1, n1, a2,
                             n2 - 1);  // In case there's any more subsequences
        }
    }
    return countIsIn(a1, n1, a2, n2 - 1);
}

// Exchange two doubles
void exchange(double& x, double& y) {
    double t = x;
    x = y;
    y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > divider come before all the other elements,
// and all the elements whose value is < divider come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= divider, or n if there is no such element, and firstLess is
// set to the index of the first element that is < divider, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > divider
//   * for firstNotGreater <= i < firstLess, a[i] == divider
//   * for firstLess <= i < n, a[i] < divider
// All the elements > divider end up in no particular order.
// All the elements < divider end up in no particular order.
void divide(double a[],
            int n,
            double divider,
            int& firstNotGreater,
            int& firstLess) {
    if (n < 0)
        n = 0;

    // It will always be the case that just before evaluating the loop
    // condition:
    //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
    //  Every element earlier than position firstNotGreater is > divider
    //  Every element from position firstNotGreater to firstUnknown-1 is
    //    == divider
    //  Every element from firstUnknown to firstLess-1 is not known yet
    //  Every element at position firstLess or later is < divider

    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess) {
        if (a[firstUnknown] < divider) {
            firstLess--;
            exchange(a[firstUnknown], a[firstLess]);
        } else {
            if (a[firstUnknown] > divider) {
                exchange(a[firstNotGreater], a[firstUnknown]);
                firstNotGreater++;
            }
            firstUnknown++;
        }
    }
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n) {
    if (n <= 0) {
        return;
    }
    // Use divider to find least element
    int i;
    int tmp;
    divide(a, n, a[n - 1], tmp, i);
    if (i == n - 1) {  // Found minimum
        return order(a, i + 1);
    } else {
        return order(a, n - 1);
    }
}

bool isOrdered(double a[]) {
    return sizeof(a) / sizeof(decltype(a[0]));
}

bool isOrdered(double a[], int n) {
    if (n <= 1) { return true; }
    return (a[n-2] >= a[n-1]) || isOrdered(a, n-1);
}

int main() {
    {
        double a2[] = {10, 50, 40, 20, 50, 40, 30};
        double a1_1[] = {10, 20, 40};
        double a1_2[] = {10, 40, 30};
        double a1_3[] = {10, 10, 40};
        double a1_4[] = {50, 40, 30};
        std::cout << "a1_1: " << countIsIn(a1_1, 3, a2, 7) << std::endl;
        std::cout << "a1_2: " << countIsIn(a1_2, 3, a2, 7) << std::endl;
        std::cout << "a1_3: " << countIsIn(a1_3, 3, a2, 7) << std::endl;
        std::cout << "a1_4: " << countIsIn(a1_4, 3, a2, 7) << std::endl;
        assert(countIsIn(a1_1, 3, a2, 7) == 1);
        assert(countIsIn(a1_2, 3, a2, 7) == 2);
        assert(countIsIn(a1_3, 3, a2, 7) == 0);
        assert(countIsIn(a1_4, 3, a2, 7) == 3);
    }
    {
        double a2[] = {10, 50, 40, 20, 50, 40, 30};
        order(a2, 7);
        for (const double a : a2) {
            std::cout << a << ",";
        }
        std::cout << std::endl;
        assert(isOrdered(a2));
    }
}