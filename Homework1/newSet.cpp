#include "newSet.h"
#include <algorithm>
#include <iostream>

Set::Set() : m_size(0), m_max_size(DEFAULT_MAX_ITEMS), m_items(new ItemType[DEFAULT_MAX_ITEMS]) {}

Set::Set(int size) : m_size(0), m_max_size(size), m_items(new ItemType[size]) {}

Set::~Set() {
    delete[] m_items;
}

bool Set::empty() const {
    return m_size == 0;
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
    if (contains(value) || m_size == m_max_size) { return false; }
    m_items[m_size] = value;
    ++m_size;
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
    int other_m_size = other.m_size;
    const int other_m_max_size = other.m_max_size;
    ItemType* other_m_items = other.m_items;

    other.m_size = m_size;
    other.m_max_size = m_max_size;
    other.m_items = m_items;

    m_size = other_m_size;
    m_max_size = other_m_max_size;
    m_items = other_m_items;
}

void Set::dump() const {
    std::cerr << "[";
    for (int i = 0; i < m_size; ++i) {
        std::cerr << m_items[i] << ",";
    }
    std::cerr << "]" << std::endl;
}