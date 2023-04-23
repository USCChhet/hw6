#include "ht.h"
#include "hash.h"
#include <unordered_map>
#include <iostream>
#include <utility>
#include <string>
#include <sstream>
#include <functional>
#include <set>
using namespace std;

struct IntHash 
{
    size_t size;
    IntHash(size_t s) {
        size = s;
    }
    HASH_INDEX_T operator()(const int& k) const
    {
        return k % size;
    }
};

struct IntHash2 
{
    // size_t size;
    IntHash2() {}
    HASH_INDEX_T operator()(const int& k) const
    {
        return k;
    }
};

int main()
{
  double loadfactor = 0.6;
  const vector<int> sizemap = 
  {
    11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421
  };
  HashTable<int, int, LinearProber<int>, IntHash2, equal_to<int>> ht(loadfactor, LinearProber<int>(), IntHash2());   
  std::set<std::pair<int, int>> items;

  for(size_t i = 0;i< sizemap.size()-1;i++)
  {
    int min = (int)floor(sizemap[i]*loadfactor);

    for(int j = 0; j<=min/2;j++)
    {
      std::pair<int,int> pair(j,j);
      //insert then delete
      ht.insert(pair);
      ht.remove(j);
      //true
      std::cout << (ht.find(j) == nullptr) << std::endl;
    }

    //true
    std::cout << (ht.size() == 0) << std::endl;

    //reinsert the same pair, loc should change
    for(int j = 0; j<(min-(min/2));j++)
    {
      std::pair<int,int> pair(j,j);
      //insert again
      ht.insert(pair);
      items.insert(pair);

      std::cout << "check0: " << std::endl;
      std::cout << ht.probe(j) << std::endl;
      std::cout << (min/2)+1+j << std::endl;
    }
    
    //true
    std::cout << "check1: " << std::endl;
    std::cout << ht.table_.size() << std::endl;
    std::cout << sizemap[i] << std::endl;
    std::cout << (ht.table_.size() == sizemap[i]) << std::endl;

    //true
    std::cout << "check2: " << std::endl;
    std::cout << ht.size() << std::endl;
    std::cout <<  items.size() << std::endl;
    std::cout << (ht.size() == items.size()) << std::endl;

    //add another should resize/rehash
    std::pair<int,int> pair((min/2)+1,(min/2)+1);
    ht.insert(pair);
    items.insert(pair);

    //true
    std::cout << (ht.table_.size() == sizemap[i+1]) << std::endl;
    //true
    std::cout << (ht.size() == items.size()) << std::endl;
    
    //should be put into the correct spot, now that
    //the "deleted" items are actually "deleted" when resizing
    std::cout << "testing: " << std::endl;
    for(int j = 0; j<=(min/2+1);j++)
    {
      if(ht.probe(j) != j)
      {
        std::cout << "not same" << std::endl;
      }
    }
  }

    
  return 0;
}
