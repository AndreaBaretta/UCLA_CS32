#include "Set.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Set s;
    assert(s.empty());
    ItemType x = "aaaa";
    assert( !s.get(42, x)  &&  x == "aaaa"); // x unchanged by get failure
    s.insert("aaaa");
    assert(s.size() == 1);
    assert(s.get(0, x)  &&  x == "aaaa");

    Set d;
    d.insert("cccc");
    d.insert("bbbb");
    d.dump();

    d.swap(s);
    d.dump();
    s.dump();
    cout << "Passed all tests" << endl;
}