#include "CardSet.h"
#include <iostream>

CardSet::CardSet() : set() {}

bool CardSet::add(unsigned long cardNumber) {
    return set.insert(cardNumber);
}

int CardSet::size() const {
    return set.size();
}

void CardSet::print() const {
    unsigned long tmp = 0;
    for (int i = 0; i < set.size(); ++i) {
        set.get(i, tmp);
        std::cout << tmp << std::endl;
    }
}