#include "Set.h"
#include <cassert>
#include <iostream>

Set::Set() : m_head(nullptr), m_size(0) {} // Default constructor

Set::Set(const Set& other) : m_head(nullptr), m_size(other.m_size) {
    if (m_size == 0) { return; } // Edge case: size of other is 0
    NodeType* p = nullptr;              // The pointer belonging to this object
    NodeType* other_p = other.m_head;   // The pointer belonging to other
    p = new NodeType{.item=other_p->item, .next=nullptr, .prev=nullptr}; // Create head
    m_head = p; // Set the head to the first created element
    if (m_size == 1) { return; } // Assuming there's items than just the head
    other_p = other_p->next;
    for (; other_p != nullptr; other_p = other_p->next) { // Cycle through. All the items should be in order already
        p->next = new NodeType{.item=other_p->item, .next=nullptr, .prev=p}; // Create new node based on data in corresponding node in other
        p = p->next;
    }
}

Set::~Set() {
    if (empty()) { return; } // Edge case: set is empty
    NodeType* p = m_head;
    while (p->next != nullptr) { // Cycle through, delete all nodes before to p
        p = p->next;
        delete p->prev;
    }
    delete p; // Last node to be deleted: the tail
}

Set& Set::operator=(const Set& rhs) {
    Set tmp = Set(rhs); // Create copy of rhs
    this->swap(tmp); // Use swap to handle switching with rhs
    return *this;
    // As soon as tmp goes out of scope, destructor gets called, cleaning up memory that used to belong to *this
}

bool Set::empty() const {
    return m_size == 0;
}

int Set::size() const {
    return m_size;
}

bool Set::insert(const ItemType& value) {
    if (empty()) { // Edge case
        m_head = new NodeType{.item=ItemType(value), .next=nullptr, .prev=nullptr};
        ++m_size;
        return true;
    }
    NodeType* p = m_head;
    while (p != nullptr) {
        if (p->item == value) { return false; } // Item already exists

        if (value < p->item) {

            NodeType* new_node = new NodeType{.item=ItemType(value), .next=p, .prev=p->prev};
            if (p->prev != nullptr) { // Inserting at head
                p->prev->next = new_node; // Should an item previous to p exist, set its next to new_node
            }

            p->prev = new_node; // p's prev now points to new_node

            if (p == m_head) { // If at head, set new head
                m_head = new_node;
            }

            ++m_size;
            return true;
        }
        if (p->next == nullptr) { // Inserting to tail
            NodeType* new_node = new NodeType{.item=ItemType(value), .next=nullptr, .prev=p};
            p->next = new_node;
            ++m_size;
            return true;
        }
        p = p->next;
    }
    assert(false); // Probably shouldn't put this in file code
                   // But you only live once, ya know?
    return false;
}

bool Set::erase(const ItemType& value) {
    if (empty()) { return false; }
    NodeType* p = m_head;
    if (p->item == value) { // Deleting head
        if (p->next != nullptr) {
            p->next->prev = nullptr;
        }
        m_head = p->next;
        delete p;
        --m_size;
        return true;
    }
    while (p->next != nullptr) { // Normal case: Malcolm's in the middle
        if (p->item == value) {
            p->prev->next = p->next;
            p->next->prev = p->prev;
            delete p;
            --m_size;
            return true;
        }
        p = p->next;
    }
    if (p->item == value) { // Deleting tail
        p->prev->next = nullptr;
        delete p;
        --m_size;
        return true;
    }
    return false;
}

bool Set::contains(const ItemType& value) const {
    if (empty()) { return false; }
    NodeType* p = m_head;
    while (p->next != nullptr) {
        if (p->item == value) { return true; }
        p = p->next;
    }
    return p->item == value;
}

bool Set::get(int pos, ItemType& value) const {
    if (pos >= m_size || pos < 0) { return false; }
    NodeType* p = m_head;
    for (int i = 1; i <= pos; ++i) {
        p = p->next;
    }
    value = ItemType(p->item);
    return true;
}

void Set::swap(Set& other) {
    NodeType* other_head = other.m_head;
    other.m_head = m_head;
    m_head = other_head;

    size_t other_size = other.m_size;
    other.m_size = m_size;
    m_size = other_size;
}

void Set::dump() const {
    int i = 0;
    ItemType item;
    std::cerr << "{";
    while (get(i++, item)) {
        std::cerr << item << ",";
    }
    std::cerr << "}" << std::endl;
}

void unite(const Set& s1, const Set& s2, Set& result) {
    Set s = Set();
    ItemType item;
    for (int i = 0; i < s1.size(); ++i) {
        s1.get(i, item);
        s.insert(item);
    }
    for (int i = 0; i < s2.size(); ++i) {
        s2.get(i, item);
        s.insert(item);
    }
    result.swap(s);
}

void butNot(const Set& s1, const Set& s2, Set& result) {
    Set s = Set();
    ItemType item;
    for (int i = 0; i < s1.size(); ++i) {
        s1.get(i, item);
        if (!s2.contains(item)) { s.insert(item); }
    }
    result.swap(s);
}