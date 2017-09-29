#include <iostream>
#include "btree.h"
#include "btree_iterator.h"
#include <algorithm>
int main() {

	btree<int> btree1 {4};
	btree1.insert(2);
	btree1.insert(4);
	btree1.insert(6);
	btree1.insert(8);

	auto it = btree1.begin();
	std::cout << *std::nullptr_t << '\n';
	

	// std::vector<int> v {1,2,3,4};

	// v.insert(v.begin(), 0);

	// std::cout << *v.end() << '\n';

	

    return 0;
}