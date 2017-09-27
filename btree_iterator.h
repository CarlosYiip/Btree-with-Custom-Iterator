//
// Created by carlos Ye on 26/9/17.
//

#ifndef A4_BTREE_btree_iterator_H
#define A4_BTREE_btree_iterator_H

#include <iterator>
#include <cassert>

template <typename T> class btree_iterator {
public:
	typedef std::ptrdiff_t difference_type;
	typedef std::bidirectional_iterator_tag btree_iterator_category;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;

	reference operator*() const { return *pointee_; };
	pointer operator->() const { return &(operator*()); }
	btree_iterator& operator++();
	btree_iterator& operator--();
	bool operator==(const btree_iterator& other) const { return pointee_ == other.pointee_; };
	bool operator!=(const btree_iterator& other) const { return !operator==(other); }

	btree_iterator(T *pointee = nullptr) : pointee_{pointee} {}
private:
	T* pointee_;
};


template <typename T>
btree_iterator<T>& btree_iterator<T>::operator++() {
	assert(pointee_ != nullptr);
	++pointee_;
	return *this; 
}

template <typename T>
btree_iterator<T>& btree_iterator<T>::operator--() {
	assert(pointee_ != nullptr);
	--pointee_;
	return *this; 
}



#endif //A4_BTREE_btree_iterator_H
