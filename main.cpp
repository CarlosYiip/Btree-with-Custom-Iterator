#include <iostream>
#include "btree.h"

int main() {
	btree<double> btree1 {};

    for (double i = 0; i < 10; ++i) {
        btree1.insert(i);
    }

    bool found  = (btree1.find(1) != btree1.end());

    std::cout << found << '\n';



    return 0;
}
