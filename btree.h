//
// Created by carlos Ye on 25/9/17.
//

#ifndef A4_Btree_H
#define A4_Btree_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "btree_iterator.h"

template <typename T>
class Btree {
public:
    Btree(size_t maxNodeElems = 40) { root.elementsPerNode = maxNodeElems; };
    Btree(const Btree<T>&);
    Btree(Btree<T>&&);

    Btree<T>& operator=(const Btree<T>&);
    Btree<T>& operator=(Btree<T>&&);

   // friend std::ostream& operator<<<>(std::ostream&, const Btree<T>&);
    friend class btree_iterator<T>;
    // friend class const_iterator<T>
    typedef btree_iterator<T> iterator;
    // typedef const_iterator<T> const_iterator;
    // iterator find(const T&);
    // const_iterator find(const T&) const;
    std::pair<iterator, bool> insert(const T&);

    ~Btree() = default;

private:
    struct Node {
        size_t elementsPerNode;
        std::vector<T> client_elems;
        std::shared_ptr<T> parent_node;
        std::vector<std::shared_ptr<Node>> child_nodes;
        bool isFull() { return client_elems.size() == elementsPerNode; }
        bool isRoot() { return parent_node == nullptr; }
        bool operator<(Node& other) { return client_elems.front() < other.client_elems.front(); }
    };
    Node root;
};





template <typename T>
std::pair<btree_iterator<T>, bool> Btree<T>::insert(const T& val) {
    Node &node = root;
    std::pair<btree_iterator<T>, bool> res;

    while (true) {
        if (!node.isFull()) {
            auto it = std::upper_bound(node.client_elems.begin(), node.client_elems.end(), val);
            if (it != node.client_elems.end() && *it == val) {
                // std::cout << "gotcha!\n";
                btree_iterator<T> it_;
                res.first = it_;
                res.second = false;
                return res;
            }
            node.client_elems.insert(it, val);
            break;
        } else {
            if (node.child_nodes.size() == 0) {
                for (size_t i = 0; i < node.elementsPerNode; ++i) {
                    Node newNode;
                    newNode.elementsPerNode = node.elementsPerNode;
                    std::shared_ptr<Node> sp = std::make_shared<Node>(newNode);
                    node.child_nodes.push_back(sp);
                }
            }

            auto it = std::upper_bound(node.client_elems.begin(), node.client_elems.end(), val);
            int dis = std::distance(node.client_elems.begin(), it);

            std::cout << dis << '\n';

            break;
            
        }
    }







    return res;
}




#endif //A4_Btree_H





