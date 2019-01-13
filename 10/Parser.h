#ifndef __PARSER_H__
#define __PARSER_H__

#include <deque>
#include "Token.h"

using namespace std;

class Parser {
  public:
    void parse(deque<Token>& tokens, string fileName);
};

#endif
