#include "Set.h"
#include <iostream>

int main() {
    Set s1{};
    s1.insert("aaa");
    s1.insert("ccc");
    s1.insert("bbb");
    s1.dump();

    Set s2{};
    s2.insert("eee");
    s2.swap(s1);
    s2.dump();
    s1.dump();

    s1 = s2;
    s1.erase("aaa");
    s1.erase("ccc");
    s1.dump();
    s1.erase("bbb");
    s1.dump();

    Set s3{};
    s3.insert("a");
    s3.insert("d");
    s3.insert("b");
    s3.insert("c");

    Set s4{};
    s4.insert("e");
    s4.insert("b");
    s4.insert("d");

    Set result{};
    unite(s3, s4, result);
    result.dump();
    unite(s4, s3, result);
    result.dump();
    butNot(s3, s4, result);
    result.dump();

    // s2.erase("aaa");
    // s2.erase("ccc");
    // s2.dump();
    // s2.erase("bbb");
    // s2.dump();
}