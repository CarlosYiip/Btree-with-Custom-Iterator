//
// Created by carlos Ye on 25/9/17.
//

#ifndef A4_BTREE_H
#define A4_BTREE_H

#include <iostream>

template <typename T>
class btree {
public:
    btree(size_t maxNodeElems = 40);
    btree(const btree<T>&);
    btree(btree<T>&&);

    btree<T>& operator=(const btree<T>&);
    btree<T>& operator=(btree<T>&&);

//    friend std::ostream& operator<<<>(std::ostream&, const btree<T>&);

    ~btree();

private:

};


#endif //A4_BTREE_H
