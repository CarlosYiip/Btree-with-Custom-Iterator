//
// Created by carlos Ye on 25/9/17.
//

#ifndef A4_btree_H
#define A4_btree_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "btree_iterator.h"


template <typename T> class btree;
template <typename T>
std::ostream& operator<<(std::ostream &os, const btree<T> &bt) {
    bt.root->inorderPrint();
    return os;
}

template <typename T>
class btree {
private:
    class Node;
    class Element;

public:
    friend std::ostream& operator<< <> (std::ostream &os, const btree<T> &bt);

    btree(size_t maxNodeElems = 40) {
        Node n {maxNodeElems};
        root = std::make_shared<Node>(n);
    };

    btree(const btree<T>&);
    btree(btree<T>&&);

    btree<T>& operator=(const btree<T>&);
    btree<T>& operator=(btree<T>&&);

   // friend std::ostream& operator<<<>(std::ostream&, const btree<T>&);
    friend class btree_iterator<T>;
    // friend class const_iterator<T>
    typedef btree_iterator<Element> iterator;
    // typedef const_iterator<T> const_iterator;
    iterator find(const T&);
    // const_iterator find(const T&) const;
    std::pair<iterator, bool> insert(const T&);

    ~btree() = default;

private:
    class Element {
    public:
        Element(const T& val_, std::shared_ptr<Node> sp_) : val{val_}, sp{sp_} {};
        Element(const Element&) = default;
        Element(Element&&) = default;
        Element& operator=(const Element&) = default;
        Element& operator=(Element&&) = default;

        bool friend operator<(const Element& lhs, const Element& rhs) {
            return lhs.getElement() < rhs.getElement();
        }
        bool friend operator==(const Element& lhs, const Element& rhs) {
            return lhs.getElement() == rhs.getElement();
        }

        const T& getElement() const { return val; }
        std::shared_ptr<Node> getNodeSp() const { return sp; }
    private:
        T val;
        std::shared_ptr<Node> sp;
    };

    class Node {
    public:
        friend class btree<T>;

        Node(size_t maxNodeElems) : elementsPerNode{maxNodeElems} {};
        Node(const Node&) = default;
        Node(Node&&) = default;
        Node& operator=(const Node&) = default;
        Node& operator=(Node&&) = default;
        ~Node() = default;

        btree_iterator<Element> insertElem(const T&);
        btree_iterator<Element> findElem(const T&) ;
        std::shared_ptr<Node> nextChildNode(const T&) const ;
        void initializeChildNodes();
        bool hasChild() const ;
        bool hasElem() const ;
        bool isInNode(const T&) const ;
        bool isFull() const ;
        bool isRoot() const ;
        void inorderPrint() const;
    private:
        size_t elementsPerNode;
        std::vector<Element> client_elems;
        std::shared_ptr<Node> parent_node;
        unsigned pos_in_parent_node;
        std::vector<std::shared_ptr<Node>> child_nodes;
    };
    std::shared_ptr<Node> root;
};

/*************************************************** btree ****************************************************/




template <typename T>
std::pair<btree_iterator<typename btree<T>::Element>, bool> btree<T>::insert(const T &val) {
    std::pair<btree_iterator<Element>, bool> res;
    std::shared_ptr<Node> node = root;

    while (true) {
        if (node->isInNode(val)) {
            res.first = node->findElem(val);
            res.second = false;
            break;
        }

        if (!node->isFull()) {
            res.first = node->insertElem(val);
            res.second = true;
            break;
        }

        if (!node->hasChild())
            node->initializeChildNodes();
        node = node->nextChildNode(val);
        break;
    }
    return res;
}
/*************************************************** Node ****************************************************/


//template <typename T>
//void btree<T>::Node::inorderPrint() const {
//
//    if (!hasChild()) {
//        for (const T elem : client_elems)
//            std:: cout << elem << ' ';
//        return;
//    }
//
//    for (int i = 0; i < elementsPerNode; ++i) {
//        child_nodes[i]->inorderPrint();
//        std::cout << client_elems[i] << ' ';
//    }
//    child_nodes[elementsPerNode]->inorderPrint();
//}



template <typename T>
btree_iterator<typename btree<T>::Element> btree<T>::Node::insertElem(const T &val) {
    Element elem {val, std::make_shared<Node>(*this)};
    auto it = std::lower_bound(client_elems.begin(), client_elems.end(), elem);
    client_elems.insert(it, elem);
    return findElem(val);
}

template <typename T>
btree_iterator<typename btree<T>::Element> btree<T>::Node::findElem(const T &val) {
    Element elem {val, std::make_shared<Node>(*this)};
    auto it = std::find(client_elems.begin(), client_elems.end(), elem);
    btree_iterator<Element> res_it {&*it};
    return res_it;
}

template <typename T>
bool btree<T>::Node::isInNode(const T& val) const {
    Element elem {val, std::make_shared<Node>(*this)};
    auto it = std::find(client_elems.cbegin(), client_elems.cend(), elem);
    return it != client_elems.cend();
}

template <typename T>
bool btree<T>::Node::isFull() const {
    return client_elems.size() == elementsPerNode;
}

template <typename T>
void btree<T>::Node::initializeChildNodes() {
    for (auto i = 0; i < elementsPerNode + 1; ++i) {
        Node newNode {elementsPerNode};
        newNode.parent_node = std::make_shared<Node>(*this);
        newNode.pos_in_parent_node = static_cast<unsigned>(i);
        std::shared_ptr<Node> sp = std::make_shared<Node>(newNode);
        sp->pos_in_parent_node = static_cast<unsigned>(i);
        child_nodes.push_back(sp);
    }
}

template <typename T>
std::shared_ptr<typename btree<T>::Node> btree<T>::Node::nextChildNode(const T &val) const {
    Element elem {val, std::make_shared<Node>(*this)};
    auto it = std::lower_bound(client_elems.cbegin(), client_elems.cend(), elem);
    int dis = std::distance(client_elems.cbegin(), it);
    return child_nodes[dis];
}

template <typename T>
bool btree<T>::Node::hasChild() const {
    return child_nodes.size() > 0;
}

template <typename T>
bool btree<T>::Node::hasElem() const {
    return client_elems.size() > 0;
}

/*************************************************** Element ****************************************************/
#endif //A4_btree_H





