//
// Created by carlos Ye on 25/9/17.
//

#ifndef A4_btree_H
#define A4_btree_H

#include <iostream>
#include <cstddef>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include "btree_iterator.h"

template <typename T>
class btree {
public:
    friend class btree_iterator<T>;
    friend class btree_const_iterator<T>;
    typedef btree_iterator<T> iterator;
    typedef btree_const_iterator<T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    btree(size_t maxNodeElems = 40);
    btree(const btree<T>&);
    btree(btree<T>&&);
    btree<T>& operator=(const btree<T>&);
    btree<T>& operator=(btree<T>&&);

    iterator find(const T&);
    const_iterator find(const T&) const;
    std::pair<iterator, bool> insert(const T&);

    iterator begin() { return iterator{head}; }

    iterator begin() const { return iterator{head}; }

    const_iterator cbegin() const { return const_iterator{head}; }

    iterator end() { return iterator{tail->next}; }

    iterator end() const { return iterator{tail->next}; }

    const_iterator cend() const { return const_iterator{tail->next}; }

    reverse_iterator rbegin() { return std::reverse_iterator<iterator>(end()); }

    reverse_iterator rbegin() const { return std::reverse_iterator<iterator>(end()); }

    const_reverse_iterator rcbegin() const { return std::reverse_iterator<const_iterator>(cend()); }

    reverse_iterator rend() { return std::reverse_iterator<iterator>(begin()); }

    reverse_iterator rend() const { return std::reverse_iterator<iterator>(begin()); }

    const_reverse_iterator rcend() const { return std::reverse_iterator<const_iterator>(cbegin()); }

    ~btree() {
        if (head != nullptr)
            delete head->prev;
        if (tail != nullptr)
            delete tail->next;
        delete root;
    };

    friend std::ostream& operator<< (std::ostream& os, const btree<T>& tree) {
        if (tree.head == nullptr)
            return os;
        for (auto it = tree.cbegin(); it != tree.cend(); ++it)
            std::cout << *it << ' ';
        return os;

    };

private:
    class Node {
    private:
        struct Element;
    public:
        friend class btree<T>;
        friend class btree_iterator<T>;
        friend class btree_const_iterator<T>;
        typedef typename Node::Element Element;

        Node(size_t maxNodeElems) : maxNodeElems_{maxNodeElems} {};
        Node(const Node&);
        Node(Node&&) = default;
        Node& operator=(const Node&) = default;
        Node& operator=(Node&&) = default;
        ~Node() {
            for (auto &p : childs)
                delete p;
        };

        typename std::list<Element>::iterator find(const Element&);
        typename std::list<Element>::const_iterator find(const Element&) const ;
        typename std::list<Element>::iterator insert(Element&);
        bool isFull() const ;
        bool hasChild() const ;
        bool isInNode(const T&) const ;
        void initializeChild();
        void join(Element&);
        Node* findChild(const T&) const ;
    private:
        struct Element {
            T elem;
            Element *prev;
            Element *next;

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
        std::list<Element> elems;
        std::vector<Node*> childs;
        std::vector<T*> vals;
    };
    typedef typename btree<T>::Node::Element Element;
    Node* root;
public:
    Element *head = nullptr;
    Element *tail = nullptr;

    void bfs_insert(const btree<T>&);
    Element* find_helper(const T&) const ;
};
/******************************************************************* Tree ****************************************************************/
template <typename T>
btree<T>::btree(size_t maxNodeElems) : root{new Node {maxNodeElems}} {}

template <typename T>
btree<T>::btree(const btree<T> &other) : root{new Node {other.root->maxNodeElems_}} {
    bfs_insert(other);
}

template <typename T>
btree<T>::btree(btree<T> &&other) {
    root = other.root;
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    other.root = new Node {root->maxNodeElems_};
}

template <typename T>
btree<T>& btree<T>::operator=(const btree<T> &other) {
    if (this != &other) {
        bfs_insert(other);
    }
    return *this;
}

template <typename T>
btree<T>& btree<T>::operator=(btree<T> &&other) {
    if (this != &other) {
        root = other.root;
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        other.root = new Node {root->maxNodeElems_};
    }
    return *this;
}

template <typename T>
typename btree<T>::iterator btree<T>::find(const T &val) {
    return iterator {find_helper(val)};
}

template <typename T>
typename btree<T>::const_iterator btree<T>::find(const T &val) const {
    return const_iterator {find_helper(val)};
}

template <typename T>
typename btree<T>::Node::Element* btree<T>::find_helper(const T &val) const {
    typedef typename btree<T>::Node::Element Element;
    Element newElem {val, nullptr, nullptr};
    Node *node = root;
    while (true) {
        if (node->isInNode(val))
            return &*node->find(newElem);
        if (!node->hasChild())
            return tail->next;
        node = node->findChild(val);
    }
}

template <typename T>
std::pair<typename btree<T>::iterator, bool> btree<T>::insert(const T &val) {
    typedef typename btree<T>::Node::Element Element;

    std::pair<iterator, bool> res_pair;
    Node *node = root;
    typename std::list<Element>::iterator it;
    Element newElem {val, nullptr, nullptr};
    while (true) {
        if (!node->isFull()) {
            if (node->isInNode(val)) {
                it = node->find(newElem);
                res_pair = {&*it, false};
            } else {
                it = node->insert(newElem);
                res_pair = {&*it, true};
            }
            break;
        } else {
            if (node->isInNode(val)) {
                it = node->find(newElem);
                res_pair = {&*it, false};
                break;
            } else {
                if (!node->hasChild())
                    node->initializeChild();
                Node *child = node->findChild(val);
                if (child->elems.size() == 0)
                    node->join(newElem);
                node = child;
            }
        }
    }

    if (head == nullptr) {
        head = &*it;
        head->prev = new Element;
        head->prev->next = head;
        head->prev->prev = nullptr;
    } else if (val < head->elem) {
        head = &*it;
    }


    if (tail == nullptr) {
        tail = &*it;
        tail->next = new Element;
        tail->next->prev = tail;
        tail->next->next = nullptr;
    } else if (val > tail->elem) {
        tail = &*it;
    }

    return res_pair;
}

template <typename T>
void btree<T>::bfs_insert(const btree<T> &other) {
    std::deque<Node*> dq {other.root};
    while (dq.size() != 0) {
        Node *next = dq.front();
        dq.pop_front();
        for (auto &e : next->elems)
            insert(e.elem);
        for (auto &c : next->childs)
            dq.push_back(c);
    }
}

/******************************************************************* Node ****************************************************************/
template <typename T>
typename std::list<typename btree<T>::Node::Element>::iterator btree<T>::Node::find(const typename btree<T>::Node::Element &newElem) {
    auto it = std::find(elems.begin(), elems.end(), newElem);
    return it;
}

template <typename T>
typename std::list<typename btree<T>::Node::Element>::const_iterator btree<T>::Node::find(const typename btree<T>::Node::Element &newElem) const {
    auto it = std::find(elems.cbegin(), elems.cend(), newElem);
    return it;
}

template <typename T>
typename std::list<typename btree<T>::Node::Element>::iterator btree<T>::Node::insert(typename btree<T>::Node::Element &newElem) {
    typedef typename btree<T>::Node::Element Element;
    typename std::list<Element>::iterator it;

    if (elems.size() == 0) {
        elems.push_back(newElem);
        it = std::prev(elems.end());
        if (newElem.prev)
            newElem.prev->next = &*it;
        if (newElem.next)
            newElem.next->prev = &*it;
    } else {
        if (newElem < elems.front()) {
            Element &oldFront = elems.front();
            elems.insert(elems.begin(), newElem);
            Element &newFront = elems.front();
            newFront.prev = &*(oldFront.prev);
            oldFront.prev = &newFront;
            newFront.next = &oldFront;
            newFront.prev->next = &newFront;
            it = elems.begin();
        } else if (elems.back() < newElem) {
            Element &oldBack = elems.back();
            elems.push_back(newElem);
            Element &newEnd = elems.back();
            newEnd.next = &*(oldBack.next);
            oldBack.next = &newEnd;
            newEnd.prev = &oldBack;
            newEnd.next->prev = &newEnd;
            it = std::prev(elems.end());
        } else {
            auto tmp_it = std::lower_bound(elems.begin(), elems.end(), newElem);
            elems.insert(tmp_it, newElem);
            Element &prev = *(std::prev(tmp_it, 2));
            Element &next = *(tmp_it);
            Element &new_ = *(std::prev(tmp_it));
            prev.next = &new_;
            next.prev = &new_;
            new_.prev = &prev;
            new_.next = &next;
            it = std::prev(tmp_it);
        }
    }

    T* p = &it->elem;
    auto val_it = std::lower_bound(vals.begin(), vals.end(), p, [&] (const T *lhs, const T *rhs) {
        return *lhs < *rhs;
    });

    vals.insert(val_it, p);
    return it;
}

template <typename T>
bool btree<T>::Node::isFull() const {
    return elems.size() >= maxNodeElems_;
};

template <typename T>
bool btree<T>::Node::hasChild() const {
    return childs.size() != 0;
};

template <typename T>
bool btree<T>::Node::isInNode(const T &val) const {
    const T *p = &val;
    auto it = std::lower_bound(vals.cbegin(), vals.cend(), p, [&] (const T *lhs, const T *rhs) {
        return *lhs < *rhs;
    });
    if (it != vals.end() && **it == val)
        return true;
    return false;
}

template <typename T>
void btree<T>::Node::initializeChild() {
    for (size_t i = 0; i < maxNodeElems_ + 1; ++i)
        childs.emplace_back(new Node {maxNodeElems_});
}

template <typename T>
typename btree<T>::Node* btree<T>::Node::findChild(const T &val) const {
    const T *p = &val;
    auto it = std::lower_bound(vals.cbegin(), vals.cend(), p, [&] (const T *lhs, const T *rhs) {
        return *lhs < *rhs;
    });
    int i = std::distance(vals.begin(), it);
    return childs[i];
}

template <typename T>
void btree<T>::Node::join(Element &newElem) {
    auto it = std::lower_bound(elems.begin(), elems.end(), newElem);
    if (it == elems.end()) {
        Element &existed = *std::prev(it);
        Element &newNext = *existed.next;
        existed.next = &newElem;
        newElem.prev = &existed;
        newElem.next = &newNext;
        newNext.prev = &newElem;
    } else {
        Element &existed = *it;
        Element &newPrev = *existed.prev;
        existed.prev = &newElem;
        newElem.next = &existed;
        newElem.prev = &newPrev;
        newPrev.next = &newElem;
    }
}

#endif //A4_btree_H