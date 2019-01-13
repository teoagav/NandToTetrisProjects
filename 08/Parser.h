#ifndef __PARSER_H__
#define __PARSER_H__

#include <vector>
#include "Command.h"

using namespace std;

class Parser {
  public:
    vector<Command> parse(string& path, string& fileName);
};

#endif
