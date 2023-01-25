#include "Set.h"
#include <algorithm>
#include <iostream>

Set::Set() : m_size(0) {}

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
    if (contains(value) || m_size == DEFAULT_MAX_ITEMS) { return false; }

    int i;
    for (i = 0; i < m_size; ++i) {
        if (value < m_items[i]) {
            break;
        }
    }
    if (i == m_size) {
        m_items[i] = ItemType(value);
        ++m_size;
    } else {
        ItemType tmp1 = value; //cur
        ItemType tmp2 = m_items[i]; //next
        for (; i < m_size; ++i) {
            tmp2 = m_items[i];
            m_items[i] = tmp1;
            tmp1 = tmp2;
        }
        m_items[i] = tmp1;
        ++m_size;
    }
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
    const Set tmp = other;
    other = *this;
    *this = tmp;
}

void Set::dump() const {
    std::cerr << "[";
    for (int i = 0; i < m_size; ++i) {
        std::cerr << m_items[i] << ",";
    }
    std::cerr << "]" << std::endl;
}