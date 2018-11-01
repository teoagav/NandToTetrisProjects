#ifndef __CODE_GEN__
#define __CODE_GEN__

#include <iostream>
#include <vector>
#include <map>
#include "instruction.h"
#include <string>

using namespace std;

class CodeGenerator {
  public:
    void generate(vector<Instruction>& lines);
};

#endif
