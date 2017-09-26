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
    Btree(size_t maxNodeElems = 40);
    Btree(const Btree<T>&);
    Btree(Btree<T>&&);

    Btree<T>& operator=(const Btree<T>&);
    Btree<T>& operator=(Btree<T>&&);

//    friend std::ostream& operator<<<>(std::ostream&, const Btree<T>&);

    typedef btree_iterator<T> iterator;
    typedef const_btree_iterator<T> const_iterator;
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


template <typename T>
Btree<T>::Btree(size_t maxNodeElems) : elementsPerNode{maxNodeElems} {}


#endif //A4_Btree_H
