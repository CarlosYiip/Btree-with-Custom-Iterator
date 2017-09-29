//
// Created by carlos Ye on 26/9/17.
//

#ifndef A4_btree_btree_iterator_H
#define A4_btree_btree_iterator_H

#include <iterator>

template <typename T> class btree;

template <typename T> class btree_iterator {
public:
	typedef std::ptrdiff_t difference_type;
	typedef std::bidirectional_iterator_tag btree_iterator_category;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;

	reference operator*() const;
	pointer operator->() const { return &(operator*()); }
	btree_iterator& operator++();
	btree_iterator& operator--();
	bool operator==(const btree_iterator& other) const { return this.pointee_ == other.pointee_; };
	bool operator!=(const btree_iterator& other) const { return !operator==(other); }

	btree_iterator(std::shared_ptr<typename btree<T>::Node::Element> pointee = nullptr) : pointee_{pointee} {}
private:
	std::shared_ptr<typename btree<T>::Node::Element> pointee_;
};


template <typename T>
T& btree_iterator<T>::operator*() const {
	return pointee_->elem;
}


template <typename T>
btree_iterator<T>& btree_iterator<T>::operator++() {
	// std::cout << (pointee_->next == nullptr) << '\n';
	pointee_ = pointee_->next;
	return *this; 
}

template <typename T>
btree_iterator<T>& btree_iterator<T>::operator--() {
	pointee_ = pointee_->prev;
	return *this; 
}



#endif //A4_btree_btree_iterator_H
