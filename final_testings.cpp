#include "btree.h"
#include <iostream>

int main()
{
	btree<int> btree1 {2};

	// Testing insert
	for (int i = 0; i < 10; ++i)
	{
		btree1.insert(i);
	}

	// Testing copy/move constructor/assignment
	btree<int> btree2 = btree1;
	btree<int> btree3 = std::move(btree1);
	btree<int> btree4;
	btree4 = btree3;
	btree<int> btree5;
	btree5 = std::move(btree2);

	// Testing begin and end
	for (auto it = btree3.begin(); it != btree3.end(); ++it)
	{
		*it += 1;
	}

	// Testing rbegin and rend
	for (auto it = btree4.rbegin(); it != btree4.rend(); ++it)
	{
		*it += 1;
	}

	// Testing output operator
	std::cout << btree1 << '\n';
	std::cout << btree2 << '\n';

	// Testing cbegin and cend
	std::copy(btree3.cbegin(), btree3.cend(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << '\n';
	// Testing rcbegin and rcend
	std::copy(btree4.rcbegin(), btree4.rcend(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << '\n';

	std::cout << btree5 << '\n';

	// Testing find
	btree<int>::const_iterator const_it = btree5.find(5);
	std::cout << (const_it == btree5.cend()) << '\n';
	btree<int>::iterator it = btree5.find(100);
	std::cout << (it == btree5.end()) << '\n';

	return 0;
}