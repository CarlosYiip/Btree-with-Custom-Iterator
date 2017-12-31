#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <map>

#include "btree.h"



int main() {
    btree<int> btree1 {1};
    for (int i = 0; i < 10; ++i) {
        btree1.insert(i / 2);
    }
    std::copy(btree1.rbegin(), btree1.rend(), std::ostream_iterator<int>(std::cout, " "));
    return 0;
}
