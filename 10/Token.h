#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

using namespace std;

class Token {
  string lexeme;
  string type;

  public:
    Token() {}
    Token(string lexeme, string type): lexeme{lexeme}, type{type} {}

    string& getLexeme() { return lexeme; }
    string& getType() { return type; }

    void setLexeme(string lexeme) { this->lexeme = lexeme; }
    void setType(string type) { this->type = type; }

};

#endif
