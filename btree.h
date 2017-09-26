//
// Created by carlos Ye on 25/9/17.
//

#ifndef A4_BTREE_H
#define A4_BTREE_H

#include <iostream>
#include "btree_iterator.h"
#include <vector>
#include <memory>

template <typename T>
class btree {
public:
    btree(size_t maxNodeElems = 40);
    btree(const btree<T>&);
    btree(btree<T>&&);

    btree<T>& operator=(const btree<T>&);
    btree<T>& operator=(btree<T>&&);

//    friend std::ostream& operator<<<>(std::ostream&, const btree<T>&);

    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;
    iterator find(const T&);
    const_iterator find(const T&) const;
    std::pair<iterator, bool> insert(const T&);

    ~btree();

private:

    struct Node {
        std::vector<T> client_elems;
        std::vector<std::unique_ptr<Node>> child_nodes;
    };


    std::vector<Node> nodes;

};


#endif //A4_BTREE_H
