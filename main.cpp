#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <utility>

#include "btree.h"

namespace {

const long kMinInteger = 1000000;
const long kMaxInteger = 100000000;

void initRandom(unsigned long);
long getRandom(long low, long high);
void insertRandomNumbers(btree<long>&, set<long>&, size_t);
bool confirmEverythingMatches(const btree<long>&, const set<long>&);

/**
 * Initialises random number generator.
 * The default argument uses a 'random' seed, but a specific 
 * seed can be supplied to reproduce the random sequence for testing.
 **/
void initRandom(unsigned long seed = 0) {
  if (seed == 0) {
    srandom(time(NULL));
  } else {
    srandom(seed);
  }
}

/**
 * Produces a random number in the given range.
 * The random() function provides better quality random numbers
 * than the older rand() function.
 **/
long getRandom(long low, long high) {
  return (low + (random() % ((high - low) + 1)));
}

/**
 * Tries to insert numbers into the specified test container, and every time 
 * the insertion succeeds (because the number wasn't previously in the btree), 
 * the same number is inserted into the off-the-shelf set 
 * class instance so we can later track the correctness of our btree.
 **/
void insertRandomNumbers(btree<long>& testContainer, set<long>& stableContainer, size_t size) {
  cout << "Let's insert up to " << size << " numbers." << endl;
  for (size_t i = 0; i < size; i++) {
    long rndNum = getRandom(kMinInteger, kMaxInteger);
    std::pair<btree<long>::iterator, bool> result = testContainer.insert(rndNum);
    if (result.second) stableContainer.insert(rndNum);
    if ((i + 1) % 100000 == 0) 
      cout << "Inserted some " << (i + 1) << " numbers thus far." << endl;
  }
  cout << endl;
}

/**
 * Confirms that the specified btree and the specified 
 * set contain exactly the same numbers.  This does so by 
 * considering all numbers that could have been added, and 
 * asserting that each of those numbers is either present 
 * in or absent from both containers.  If all checks out, 
 * we return 0 to signal success; if along the way we see 
 * an integer in one container and not the other, then 
 * we bail immediately and return one to express failure.
 **/
bool confirmEverythingMatches(const btree<long>& testContainer, const set<long>& stableContainer) {
  cout << "Confirms the btree and the set " 
          "contain exactly the same values..." << endl;
  for (long i = kMinInteger; i <= kMaxInteger; i++) {
    bool foundInTree = (testContainer.find(i) != testContainer.end());
    bool foundInSet = (stableContainer.find(i) != stableContainer.end());
    if (foundInTree != foundInSet) {
      cout << "- btree and set don't contain the same data!" << endl; 
      cout << "Mismatch at element: " << i << endl;
      return false;
    }
  }
  cout << "- btree checks out just fine." << endl;

  return true;
}

}
