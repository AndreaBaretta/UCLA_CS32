#include "CardSet.h"
#include <iostream>
#include <cassert>

int main() {
    CardSet cardSet{};
    cardSet.add(32894);
    cardSet.add(123);
    cardSet.add(123987);
    assert(cardSet.size() == 3);
    cardSet.print();
}