//
// Created by carlos Ye on 25/9/17.
//

#ifndef A4_Btree_H
#define A4_Btree_H

#include <iostream>
#include <vector>
#include <memory>
#include "btree_iterator.h"

template <typename T>
class Btree {
public:
    Btree(size_t maxNodeElems = 40) : elementsPerNode{maxNodeElems} {};
    Btree(const Btree<T>&);
    Btree(Btree<T>&&);

    Btree<T>& operator=(const Btree<T>&);
    Btree<T>& operator=(Btree<T>&&);

//    friend std::ostream& operator<<<>(std::ostream&, const Btree<T>&);

    typedef Btree_iterator<T, T*> iterator;
    typedef Btree_iterator<T, const T*> const_iterator;
    iterator find(const T&);
    const_iterator find(const T&) const;
    std::pair<iterator, bool> insert(const T&);

    ~Btree() = default;

private:

    struct Node {
        std::vector<T> client_elems;
        std::vector<std::unique_ptr<Node>> child_nodes;
    };


    std::vector<Node> nodes;
    size_t elementsPerNode;

};


#endif //A4_Btree_H
