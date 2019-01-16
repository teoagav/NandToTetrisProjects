#ifndef __PARSER_H__
#define __PARSER_H__

#include <deque>
#include "Token.h"

using namespace std;

class ParserCodeGen {
  public:
    void parseGenerate(deque<Token>& tokens, string fileName);
};

#endif
