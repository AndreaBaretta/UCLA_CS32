#ifndef SET_H
#define SET_H

#include <string>

using ItemType = std::string;

class Set {
  private:
    struct NodeType {
        ItemType item;
        NodeType* next;
        NodeType* prev;
    };

    NodeType* m_head;
    int m_size;

  public:
    Set();
    Set(const Set& other);
    ~Set();
    Set& operator=(const Set& rhs);
    bool empty() const;
    int size() const;
    bool insert(const ItemType& value);
    bool erase(const ItemType& value);
    bool contains(const ItemType& value) const;
    bool get(int pos, ItemType& value) const;
    void swap(Set& other);
    void dump() const; // TODO: REMOVE BOYO
};

void unite(const Set& s1, const Set& s2, Set& result);

void butNot(const Set& s1, const Set& s2, Set& result);

#endif