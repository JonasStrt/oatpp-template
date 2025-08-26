#ifndef TESTCODE_HPP
#define TESTCODE_HPP

#include <string>
#include <memory>
#include <vector>
#include <iostream>

namespace model {

class TestCode {
private:


public:
   void addToArray(int*& array, size_t& length, int value);
   void floatToBinary(float* f);
};

} 

#endif 
