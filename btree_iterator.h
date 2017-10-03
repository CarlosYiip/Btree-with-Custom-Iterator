//
// Created by carlos Ye on 26/9/17.
//

#ifndef A4_btree_btree_iterator_H
#define A4_btree_btree_iterator_H

#include <iterator>

template <typename T> class btree;
template <typename T> class btree_iterator;
template <typename T> class btree_const_iterator;
template <typename T>
class btree_iterator {
public:
	typedef int difference_type;
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	friend class btree_const_iterator<T>;
	typedef typename btree<T>::Node::Element Element;
	reference operator*() const { return pointee_->elem; };
	pointer operator->() const { return &(operator*()); }
	btree_iterator& operator++() { pointee_ = pointee_->next; return *this; };
	btree_iterator& operator--() { pointee_ = pointee_->prev; return *this; };
    btree_iterator operator++(int) {
        btree_iterator it = *this;
        ++*this;
        return it;
    };
    btree_iterator operator--(int) {
        btree_iterator it = *this;
        --*this;
        return it;
    };
	bool operator==(const btree_iterator& other) const { return pointee_ == other.pointee_; };
	bool operator!=(const btree_iterator& other) const { return !operator==(other); }
	btree_iterator(Element *pointee = nullptr) : pointee_ {pointee} {}
private:
	Element *pointee_;
};

template <typename T>
class btree_const_iterator {
public:
	typedef int difference_type;
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef const T value_type;
	typedef const T* pointer;
	typedef const T& reference;
    friend class btree_iterator<T>;
	typedef typename btree<T>::Node::Element Element;
	reference operator*() const { return pointee_->elem; };
	pointer operator->() const { return &(operator*()); }
	btree_const_iterator& operator++() { pointee_ = pointee_->next; return *this; };
	btree_const_iterator& operator--() { pointee_ = pointee_->prev; return *this; };
	bool operator==(const btree_const_iterator& other) const { return pointee_ == other.pointee_; };
	bool operator!=(const btree_const_iterator& other) const { return !operator==(other); }
	btree_const_iterator(Element *pointee = nullptr) : pointee_ {pointee} {}
	btree_const_iterator(const btree_iterator<T> &other) : pointee_ {other.pointee_} {};
private:
	Element *pointee_;
};
#endif //A4_btree_btree_iterator_H
