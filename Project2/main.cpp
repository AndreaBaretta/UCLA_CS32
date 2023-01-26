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

    // s2.erase("aaa");
    // s2.erase("ccc");
    // s2.dump();
    // s2.erase("bbb");
    // s2.dump();
}