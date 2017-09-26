#include <iostream>
#include "btree.h"

int main() {

    Btree<int> btree1{4};

    std::pair<Btree<int>::iterator, bool> p;
    p = btree1.insert(1);
    p = btree1.insert(2);
    p = btree1.insert(3);
    p = btree1.insert(4);
    p = btree1.insert(5);
    std::cout << *(p.first) << p.second << '\n';
    // std::vector<double> v {0,2,3,4};
    // auto it = std::upper_bound(v.begin(), v.end(), 4);
    // std::cout << (it == v.end()) << '\n';
    



    return 0;
}