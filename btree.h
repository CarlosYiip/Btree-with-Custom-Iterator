//
// Created by carlos Ye on 25/9/17.
//

#ifndef A4_btree_H
#define A4_btree_H

#include <iostream>
#include <cstddef>
#include <vector>
#include <list>
#include <memory>
#include <algorithm>
#include "btree_iterator.h"

template <typename T>
class btree {
public:
    btree(size_t maxNodeElems = 40);
    btree(const btree<T>&);
    btree(btree<T>&&);
    btree<T>& operator=(const btree<T>&);
    btree<T>& operator=(btree<T>&&);

    friend std::ostream& operator<< (std::ostream& os, const btree<T>& tree) {
        for (auto it = tree.cbegin(); it != tree.cend(); ++it)
            std::cout << *it << ' ';
        return os;

    };

    friend class btree_iterator<T>;
    friend class btree_const_iterator<T>;
    friend class btree_riterator<T>;
    friend class btree_rconst_iterator<T>;
    typedef btree_iterator<T> iterator;
    typedef btree_const_iterator<T> const_iterator;
    typedef btree_riterator<T> reverse_iterator;
    typedef btree_const_riterator<T> const_reverse_iterator;

    iterator find(const T&);

    const_iterator find(const T&) const;

    std::pair<iterator, bool> insert(const T&);

    iterator begin() { return iterator{head}; }

    iterator begin() const { return iterator{head}; }

    const_iterator cbegin() const { return const_iterator{head}; }

    iterator end() { return iterator{nullptr}; }

    iterator end() const { return iterator{nullptr}; }

    const_iterator cend() const { return const_iterator{nullptr}; }

    reverse_iterator rbegin() { return reverse_iterator{tail}; }

    reverse_iterator rbegin() const { return reverse_iterator{tail}; }

    reverse_iterator rend() { return reverse_iterator{nullptr}; }

    reverse_iterator rend() const { return reverse_iterator{nullptr}; }

    const_reverse_iterator rcbegin() const { return const_reverse_iterator{tail}; }

    const_reverse_iterator rcend() const { return const_reverse_iterator{nullptr}; }

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
        ~Node() = default;

        typedef typename Node::Element Element;

        typename std::list<Element>::iterator find(const Element&);
        typename std::list<Element>::const_iterator find(const Element&) const ;
        typename std::list<Element>::iterator insert(Element&);

        bool isFull() const ;
        bool hasChild() const ;
        bool isInNode(const Element&) const ;

        void initializeChild();
        void join(Element&);
        Node* findChild(const Element &) const ;

        void debug() const {
            for (auto &e : elems)
                std::cout << &e << ' ' << e.elem << '\n';
            std::cout << '\n';
        }

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

            ~Element() = default;
        };

        size_t maxNodeElems_;
        std::list<Element> elems;
        std::vector<Node*> childs;
    };

    typename Node::Element *head;
    typename Node::Element *tail;
    Node* root;
};
/******************************************************************* Tree ****************************************************************/
template <typename T>
btree<T>::btree(size_t maxNodeElems) : root{new Node {maxNodeElems}} {
    head = nullptr;
    tail = nullptr;
}

template <typename T>
btree<T>::btree(const btree<T> &other) : root{new Node {other.root->maxNodeElems_}} {
    head = nullptr;
    tail = nullptr;
    for (auto it = other.cbegin(); it != other.cend(); ++it)
        insert(*it);
}

template <typename T>
btree<T>::btree(btree<T> &&other) : root{other.root}, head {other.head}, tail {other.tail} {
    other.root = new Node {root->maxNodeElems_};
    other.head = nullptr;
    other.tail = nullptr;
}

template <typename T>
btree<T>& btree<T>::operator=(const btree<T> &other) {
    if (&*this->root != &*other.root) {
        for (auto it = other.cbegin(); it != other.cend(); ++it)
            insert(*it);
    }
    return *this;
}

template <typename T>
btree<T>& btree<T>::operator=(btree<T> &&other) {
    if (&*this->root != &*other.root) {
        root = other.root;
        head = other.head;
        tail = other.tail;
        other.root = new Node {root->maxNodeElems_};
        other.head = nullptr;
        other.tail = nullptr;
    }
    return *this;
}

template <typename T>
typename btree<T>::iterator btree<T>::find(const T &val) {
    typedef typename btree<T>::Node::Element Element;
    Element newElem {val, nullptr, nullptr};
    Node *node = root;

    while (true) {
        if (node->isInNode(newElem)) {
            return iterator {&*node->find(newElem)};
        } else {
            if (!node->hasChild()) {
                return end();
            } else {
                node = node->findChild(newElem);
            }
        }
    }
}

template <typename T>
typename btree<T>::const_iterator btree<T>::find(const T &val) const {
    typedef typename btree<T>::Node::Element Element;
    Element newElem {val, nullptr, nullptr};
    Node *node = root;

    while (true) {
        if (node->isInNode(newElem)) {
            return const_iterator {&*node->find(newElem)};
        } else {
            if (!node->hasChild()) {
                return end();
            } else {
                node = node->findChild(newElem);
            }
        }
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
            if (node->isInNode(newElem)) {
                it = node->find(newElem);
                res_pair.second = false;
            } else {
                it = node->insert(newElem);
                res_pair.second = true;
            }
            break;
        } else {
            if (!node->hasChild())
                node->initializeChild();
            Node *child = node->findChild(newElem);
            if (child->elems.size() == 0)
                node->join(newElem);
            node = child;

        }
    }
    if (head == nullptr || val < head->elem)
        head = &*it;

    if (tail == nullptr || val > tail->elem)
        tail = &*it;

    res_pair.first = &*it;
    return res_pair;
}

/******************************************************************* Node ****************************************************************/
template <typename T>
typename std::list<typename btree<T>::Node::Element>::iterator btree<T>::Node::find(const typename btree<T>::Node::Element &newElem) {
    auto it = std::lower_bound(elems.begin(), elems.end(), newElem);
    return it;
}

template <typename T>
typename std::list<typename btree<T>::Node::Element>::const_iterator btree<T>::Node::find(const typename btree<T>::Node::Element &newElem) const {
    auto it = std::lower_bound(elems.cbegin(), elems.cend(), newElem);
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

            if (!oldFront.hasPrev()) {
                newFront.next = &oldFront;
                oldFront.prev = &newFront;
            } else {
                newFront.prev = &*(oldFront.prev);
                oldFront.prev = &newFront;
                newFront.next = &oldFront;
                newFront.prev->next = &newFront;
            }

            it = elems.begin();
        } else if (elems.back() < newElem) {
            Element &oldBack = elems.back();
            elems.push_back(newElem);
            Element &newEnd = elems.back();

            if (!oldBack.hasNext()) {
                oldBack.next = &newEnd;
                newEnd.prev = &oldBack;
            } else {
                newEnd.next = &*(oldBack.next);
                oldBack.next = &newEnd;
                newEnd.prev = &oldBack;
                newEnd.next->prev = &newEnd;
            }

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
bool btree<T>::Node::isInNode(const typename btree<T>::Node::Element &newElem) const {
    auto it = std::find(elems.cbegin(), elems.cend(), newElem);
    return it != elems.cend();
}

template <typename T>
void btree<T>::Node::initializeChild() {
    for (int i = 0; i < maxNodeElems_ + 1; ++i)
        childs.push_back(new Node {maxNodeElems_});
}

template <typename T>
typename btree<T>::Node* btree<T>::Node::findChild(const typename btree<T>::Node::Element &newElem) const {
    auto it = std::lower_bound(elems.begin(), elems.end(), newElem);
    int i = std::distance(elems.begin(), it);
    return childs[i];

}

template <typename T>
void btree<T>::Node::join(Element &newElem) {
    auto it = std::lower_bound(elems.begin(), elems.end(), newElem);

    if (it == elems.end()) {
        Element &existed = *std::prev(it);
        if (existed.hasNext()) {
            Element &newNext = *existed.next;
            existed.next = &newElem;
            newElem.prev = &existed;
            newElem.next = &newNext;
            newNext.prev = &newElem;
        } else {
            existed.next = &newElem;
            newElem.prev = &existed;
        }
    } else {
        Element &existed = *it;
        if (existed.hasPrev()) {
            Element &newPrev = *existed.prev;
            existed.prev = &newElem;
            newElem.next = &existed;
            newElem.prev = &newPrev;
            newPrev.next = &newElem;
        }
        existed.prev = &newElem;
        newElem.next = &existed;
    }
}


#endif //A4_btree_H