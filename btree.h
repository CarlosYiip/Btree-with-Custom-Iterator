//
// Created by carlos Ye on 25/9/17.
//

#ifndef A4_btree_H
#define A4_btree_H

#include <iostream>
#include <cstddef>
#include <vector>
#include <memory>
#include <algorithm>
#include "btree_iterator.h"

template <typename T>
class btree {
public:
    btree(size_t maxNodeElems = 40) : root{std::make_shared<Node>(Node {maxNodeElems})} {};
    btree(const btree<T>&);
    btree(btree<T>&&);
    btree<T>& operator=(const btree<T>&);
    btree<T>& operator=(btree<T>&&);

    friend class btree_iterator<T>;
    typedef btree_iterator<T> iterator;
    typedef btree_iterator<const T> const_iterator;

    iterator find(const T&);

    const_iterator find(const T&) const;

    std::pair<iterator, bool> insert(const T&);

    iterator begin() { return iterator{head}; }

    const_iterator cbegin() { return const_iterator{head}; }

    iterator end() { return iterator{nullptr}; }

    const_iterator cend() { return const_iterator{nullptr}; }

    ~btree() = default;

private:
    class Node {
    private:
        struct Element;
    public:
        friend class btree_iterator<T>;
        friend class btree<T>;

        Node(size_t maxNodeElems) : maxNodeElems_{maxNodeElems} {};
        Node(const Node&) = default;
        Node(Node&&) = default;
        Node& operator=(const Node&) = default;
        Node& operator=(Node&&) = default;
        ~Node() {};

        typename std::vector<Element>::iterator find(const T&);
        typename std::vector<Element>::const_iterator find(const T&) const ;
        typename std::vector<Element>::iterator insert(const T&);

        bool isFull() const ;
        bool hasChild() const ;
        bool isInNode(const T&) const ;

        void initializeChild();

    private:
        struct Element {
            T elem;
            std::shared_ptr<Element> prev;
            std::shared_ptr<Element> next;

            bool hasPrev() { return prev != nullptr; }
            bool hasNext() { return next != nullptr; }

            friend bool operator==(const Element& lhs, const Element& rhs) {
                return lhs.elem == rhs.elem;
            }

            friend bool operator<(const Element& lhs, const Element& rhs) {
                return lhs.elem < rhs.elem;
            }
        };

        size_t maxNodeElems_;
        std::vector<Element> elems;
        std::vector<std::shared_ptr<Node>> childs;
    };

    std::shared_ptr<typename Node::Element> head;
    std::shared_ptr<typename Node::Element> tail;
    std::shared_ptr<Node> root;
};
/******************************************************************* Tree ****************************************************************/
template <typename T>
typename btree<T>::iterator btree<T>::find(const T&) {

}

template <typename T>
typename btree<T>::const_iterator btree<T>::find(const T&) const {
    
}

template <typename T>
std::pair<typename btree<T>::iterator, bool> btree<T>::insert(const T &val) {
    typedef typename btree<T>::Node::Element Element;

    std::pair<iterator, bool> res_pair;
    std::shared_ptr<Node> node = root;
    typename std::vector<Element>::iterator it;


    while (true) {
        if (!node->isFull()) {
            if (node->isInNode(val)) {
                it = node->find(val);
                res_pair.second = false;
            } else {
                it = node->insert(val);
                res_pair.second = true;

                if (head == nullptr || val < head->elem) 
                    head = std::make_shared<Element>(*it);
                
                if (tail == nullptr || tail->elem < val)
                    tail = std::make_shared<Element>(*it);
            }
        }
        break;
    }

    res_pair.first = std::make_shared<Element>(*it);
    return res_pair;
}

/******************************************************************* Node ****************************************************************/
template <typename T>
typename std::vector<typename btree<T>::Node::Element>::iterator btree<T>::Node::find(const T &val) {
    auto it = std::lower_bound(elems.begin(), elems.end(), Element {val});
    return it;
}

template <typename T>
typename std::vector<typename btree<T>::Node::Element>::const_iterator btree<T>::Node::find(const T &val) const {
    auto it = std::lower_bound(elems.cbegin(), elems.cend(), Element {val});
    return it;
}

template <typename T>
typename std::vector<typename btree<T>::Node::Element>::iterator btree<T>::Node::insert(const T &val) {
    typedef typename btree<T>::Node::Element Element;
    typename std::vector<Element>::iterator it;
    Element newElem {val};


    if (elems.size() == 0) {
        elems.push_back(newElem);
        it = --elems.end();
    } else {
        if (newElem < elems.front()) {
            elems.insert(elems.begin(), newElem);
            it == elems.begin();
        } else if (elems.back() < newElem) {
            elems.push_back(newElem);
            it = --elems.end();
        } else {

        }
    }


    it = std::lower_bound(elems.begin(), elems.end(), Element {val});
    return it;
}

template <typename T>
bool btree<T>::Node::isFull() const {
    return elems.size() >= maxNodeElems_;
};

template <typename T>
bool btree<T>::Node::hasChild() const {
    return false;
};


template <typename T>
bool btree<T>::Node::isInNode(const T &val) const {
    return std::binary_search(elems.begin(), elems.end(), Element {val});
}

template <typename T>
void initializeChild() {

} 














#endif //A4_btree_H





