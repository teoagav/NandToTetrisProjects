#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <deque>
#include "Token.h"

using namespace std;

class Tokenizer {
  public:
    deque<Token> tokenize(string& path, string& fileName);
};

#endif
