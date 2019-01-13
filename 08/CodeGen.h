#ifndef __CODE_GEN_H__
#define __CODE_GEN_H__

#include <vector>
#include "Command.h"

using namespace std;

class CodeGen {
  public:
    void generateCode(vector<Command>& commands);
};

#endif
