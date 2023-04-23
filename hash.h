#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
//using tolower
#include <cstring>
//using isdigit
#include <ctype.h>
#include <stdio.h>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash 
{
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    
    MyStringHash(bool debug = true)
    {
      if(false == debug)
      {
        generateRValues();
      }
    }

    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {     
      //get length of k
      size_t klen = k.length();
      size_t hash = 0;

      //declare necessary arrays
      unsigned long long w[5] = {0, 0, 0, 0, 0};
      int j = 4;
      int l = 0;
      
      //convert to base36 and save into w
      for(int i = klen - 1; i >= 0; i--)
      {
        size_t exp = convertToBase36(l);
        
        //logic for digit
        if(isdigit(k.at(i)))
        {
          char c = (k.at(i));
          size_t a = (c - 22);
          w[j] += a * exp;
        }

        //logic for letter
        else if(isalpha(k.at(i)))
        {
          char c = tolower(k.at(i));
          size_t a = (c - 97);
          w[j] += a * exp;
        }

        //increment
        l++;

        //reset
        if(l == 6)
        {
          j--;
          l = 0;
        }
      }

      //compute the hash value
      for(int i = 0; i < 5; i++)
      {
        hash += (rValues[i] * w[i]);
      }

      return hash;
    }

    HASH_INDEX_T convertToBase36(int n) const
    {
      unsigned long long x = 1;
      for(int i = 0; i < n; i++)
      {
        x *= 36;
      }
      return x;
    }

    // Code to generate the random R values
    void generateRValues()
    {
      // obtain a seed from the system clock:
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
      std::mt19937 generator (seed);  // mt19937 is a standard random number generator

      // Simply call generator() [it has an operator()] to get another random number
      for(int i{ 0 }; i < 5; ++i)
      {
        rValues[i] = generator();
      }
    }
};

#endif
