#include "Set.h"
#include <algorithm>
#include <iostream>

Set::Set() : m_size(0) {}

bool Set::empty() const {
    return m_size != 0;
}

int Set::size() const {
    return m_size;
}

bool Set::contains(const ItemType& value) const {
    for (int i = 0; i < m_size; ++i) {
        if (m_items[i] == value) {
            return true;
        }
    }
    return false;
}

bool Set::insert(const ItemType& value) {
    if (contains(value)) { return false; }
    m_items[m_size] = value;
    std::sort(m_items, m_items + m_size);
    return true;
}

bool Set::erase(const ItemType& value) {
    int i;
    for (i = 0; i < m_size; ++i) {
        if (m_items[i] == value) {
            break;
        }
    }
    if (i == m_size) { return false; }
    for (; i < m_size - 1; ++i) {
        m_items[i] = m_items[i + 1];
    }
    --m_size;
    return true;
}

bool Set::get(int i, ItemType& value) const {
    if (i < 0 || i >= m_size) { return false; }
    value = ItemType(m_items[i]);
    return true;
}

void Set::swap(Set& other) {
    int iter = std::max(m_size, other.m_size);
    ItemType item1;
    ItemType item2;
    for (int i = 0; i < iter; ++i) {
        item1 = m_items[i];
        item2 = other.m_items[i];
        m_items[i] = item2;
        other.m_items[i] = item1;
    }
}

void Set::dump() const {
    std::cout << "[";
    for (int i = 0; i < m_size; ++i) {
        std::cout << m_items[i] << ",";
    }
    std::cout << "]" << std::endl;
}