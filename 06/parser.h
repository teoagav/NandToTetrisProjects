#ifndef __PARSER__
#define __PARSER__

#include <iostream>
#include <vector>
#include <map>
#include "instruction.h"
#include <string>

using namespace std;

class Parser {
  public:
    vector<Instruction> parse(string& fileName);
};

#endif
