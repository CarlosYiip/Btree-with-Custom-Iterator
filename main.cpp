#include <iostream>
#include "btree.h"
#include <list>

int main() {

    btree<int> btree1{4};
//    for (int i = 0; i < 1000000; ++i) {
//        btree1.insert(i / 2);
//    }
    std::pair<btree<int>::iterator, bool> p;
    p = btree1.insert(2);
    p = btree1.insert(4);
    p = btree1.insert(6);
    p = btree1.insert(8);
    p = btree1.insert(1);
//    p = btree1.insert(3);
//    p = btree1.insert(5);
//    p = btree1.insert(7);
//    p = btree1.insert(9);


    std::cout << p.first->getElement();
//    std::cout << typeid(p.first).name() << '\n';

//    std::cout << btree1 << '\n';

    return 0;
}