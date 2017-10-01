//
// Created by carlos Ye on 26/9/17.
//

#ifndef A4_btree_btree_iterator_H
#define A4_btree_btree_iterator_H

#include <iterator>

template <typename T> class btree;
template <typename T> class btree_const_iterator;
template <typename T> class btree_rconst_iterator;



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

	reference operator*() const;
	pointer operator->() const { return &(operator*()); }
	btree_iterator& operator++();
	btree_iterator& operator--();
	bool operator==(const btree_iterator& other) const { return pointee_ == other.pointee_; };
	bool operator!=(const btree_iterator& other) const { return !operator==(other); }

	btree_iterator(Element *pointee = nullptr) : pointee_ {pointee} {}
private:
	Element *pointee_;

};


template <typename T>
T& btree_iterator<T>::operator*() const {
	return pointee_->elem;
}


template <typename T>
btree_iterator<T>& btree_iterator<T>::operator++() {
	pointee_ = pointee_->next;
	return *this; 
}

template <typename T>
btree_iterator<T>& btree_iterator<T>::operator--() {
	pointee_ = pointee_->prev;
	return *this; 
}

template <typename T>
class btree_riterator {
public:
	typedef int difference_type;
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;

	friend class btree_rconst_iterator<T>;
	typedef typename btree<T>::Node::Element Element;

	reference operator*() const;
	pointer operator->() const { return &(operator*()); }
	btree_riterator& operator++();
	btree_riterator& operator--();
	bool operator==(const btree_riterator& other) const { return pointee_ == other.pointee_; };
	bool operator!=(const btree_riterator& other) const { return !operator==(other); }

	btree_riterator(Element *pointee = nullptr) : pointee_ {pointee} {}
private:
	Element *pointee_;

};


template <typename T>
T& btree_riterator<T>::operator*() const {
	return pointee_->elem;
}


template <typename T>
btree_riterator<T>& btree_riterator<T>::operator++() {
	pointee_ = pointee_->prev;
	return *this;
}

template <typename T>
btree_riterator<T>& btree_riterator<T>::operator--() {
	pointee_ = pointee_->next;
	return *this;
}

template <typename T>
class btree_const_iterator {
public:
	typedef int difference_type;
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef const T value_type;
	typedef const T * pointer;
	typedef const T & reference;

	typedef typename btree<T>::Node::Element Element;

	reference operator*() const;
	pointer operator->() const { return &(operator*()); }
	btree_const_iterator& operator++();
	btree_const_iterator& operator--();
	bool operator==(const btree_const_iterator& other) const { return pointee_ == other.pointee_; };
	bool operator!=(const btree_const_iterator& other) const { return !operator==(other); }

	btree_const_iterator(Element *pointee = nullptr) : pointee_ {pointee} {}
	btree_const_iterator(const btree_iterator<T> &other) : pointee_ {other.pointee_} {};
private:
	Element *pointee_;

};


template <typename T>
const T& btree_const_iterator<T>::operator*() const {
	return pointee_->elem;
}


template <typename T>
btree_const_iterator<T>& btree_const_iterator<T>::operator++() {
	pointee_ = pointee_->next;
	return *this;
}

template <typename T>
btree_const_iterator<T>& btree_const_iterator<T>::operator--() {
	pointee_ = pointee_->prev;
	return *this;
}

template <typename T>
class btree_const_riterator {
public:
	typedef int difference_type;
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef const T value_type;
	typedef const T * pointer;
	typedef const T & reference;

	typedef typename btree<T>::Node::Element Element;

	reference operator*() const;
	pointer operator->() const { return &(operator*()); }
	btree_const_riterator& operator++();
	btree_const_riterator& operator--();
	bool operator==(const btree_const_riterator& other) const { return pointee_ == other.pointee_; };
	bool operator!=(const btree_const_riterator& other) const { return !operator==(other); }

	btree_const_riterator(Element *pointee = nullptr) : pointee_ {pointee} {}
	btree_const_riterator(const btree_riterator<T> &other) : pointee_ {other.pointee_} {};
private:
	Element *pointee_;

};


template <typename T>
const T& btree_const_riterator<T>::operator*() const {
	return pointee_->elem;
}


template <typename T>
btree_const_riterator<T>& btree_const_riterator<T>::operator++() {
	pointee_ = pointee_->prev;
	return *this;
}

template <typename T>
btree_const_riterator<T>& btree_const_riterator<T>::operator--() {
	pointee_ = pointee_->next;
	return *this;
}

#endif //A4_btree_btree_iterator_H
