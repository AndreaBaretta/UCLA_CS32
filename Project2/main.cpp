#include "Set.h"
#include <iostream>
#include <cassert>
#include <type_traits>

#define CHECKTYPE(c, f, r, a)  \
{  \
    static_assert(std::is_same<decltype(&c::f), r (c::*)a>::value, \
        "You did not declare " #c "::" #f " as the Project 2 spec does");  \
    auto p = static_cast<r (c::*)a>(&c::f);  \
    (void) p;  \
}
#define CHECKTYPENONMEMBER(f, r, a)  \
{  \
    static_assert(std::is_same<decltype(&f), r (*)a>::value, \
        "You did not declare " #f " as the Project 2 spec does");  \
    auto p = static_cast<r (*)a>(f);  \
    (void) p;  \
}

static_assert(std::is_default_constructible<Set>::value,
        "Set must be default-constructible.");
static_assert(std::is_copy_constructible<Set>::value,
        "Set must be copy-constructible.");
static_assert(std::is_copy_assignable<Set>::value,
        "Set must be assignable.");

void thisFunctionWillNeverBeCalled() {
    CHECKTYPE(Set, empty,     bool, () const);
    CHECKTYPE(Set, size,      int,  () const);
    CHECKTYPE(Set, insert,    bool, (const ItemType&));
    CHECKTYPE(Set, erase,     bool, (const ItemType&));
    CHECKTYPE(Set, contains,  bool, (const ItemType&) const);
    CHECKTYPE(Set, get,       bool, (int, ItemType&) const);
    CHECKTYPE(Set, swap,      void, (Set&));
    CHECKTYPENONMEMBER(unite,  void, (const Set&, const Set&, Set&));
    CHECKTYPENONMEMBER(butNot, void, (const Set&, const Set&, Set&));
}

void test() {
    Set ss;
    assert(ss.insert("roti"));
    assert(ss.insert("pita"));
    assert(ss.size() == 2);
    assert(ss.contains("pita"));
    ItemType x = "laobing";
    assert(ss.get(0, x)  &&  x == "pita");
    assert(ss.get(1, x)  &&  x == "roti");
}

int main() {
    test();

    Set ss;  // ItemType is std::string
    ss.insert("ccc");
    ss.insert("aaa");
    ss.insert("bbb");
    ItemType x = "xxx";
    assert(!ss.get(3, x)  &&  x == "xxx");  // x is unchanged
    assert(ss.get(1, x)  &&  x == "bbb");   // "bbb" is greater than
                                            // exactly 1 item

    Set s1;
    s1.insert("aaa");
    s1.insert("ccc");
    s1.insert("bbb");
    std::string test;
    s1.get(1, test);
    assert(test == std::string("bbb")); // Tests correct order of insertion
    s1.dump();

    Set s2;
    s2.insert("eee");
    s2.swap(s1);
    assert(s2.size() == 3 || s1.size() == 1);  // Tests swapping functionality
    s1.dump();

    s1 = s2;
    assert(s1.size() == 3);
    s1.erase("aaa");
    s1.erase("ccc");
    assert(s1.size() == 1); // Tests proper erasure of elements from list
    s1.dump();
    s1.erase("bbb");
    assert(s1.size() == 0); // Tests proper erasure of elements from list
    s1.dump();

    Set s3;
    s3.insert("a");
    s3.insert("d");
    s3.insert("b");
    s3.insert("c");

    Set s4;
    s4.insert("e");
    s4.insert("b");
    s4.insert("d");

    Set result;
    unite(s3, s4, result);
    assert(result.size() == 5); // Checks to see if uniting works
    result.dump();
    unite(s4, s3, result);
    assert(result.size() == 5); // Operation should be commutative
    result.dump();
    butNot(s3, s4, result);
    assert(result.size() == 2); // Checks to see if butNot works
    result.dump();
    butNot(s4, s4, s4);
    assert(s4.size() == 0);
    s4.dump();
    unite(s3, s3, s3);
    assert(s3.size() == 4);
    s3.dump();
}