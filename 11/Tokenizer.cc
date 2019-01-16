#include "Tokenizer.h"
#include <fstream>
#include <stdlib.h>
#include <iostream>

using namespace std;

string identifyLexeme(string& lexeme) {
  if(lexeme == "class" || lexeme == "constructor" || lexeme == "function" ||
    lexeme == "method" || lexeme == "field" || lexeme == "static" ||
    lexeme == "var" || lexeme == "int" || lexeme == "char" ||
    lexeme == "boolean" || lexeme == "void" || lexeme == "true" ||
    lexeme == "false" || lexeme == "null" || lexeme == "this" ||
    lexeme == "let" || lexeme == "do" || lexeme == "if" ||
    lexeme == "else" || lexeme == "while" || lexeme == "return") {

    return "keyword";
  } else if(lexeme.length() > 1 && lexeme[0] == '"' && lexeme[lexeme.length() - 1] == '"') {
    return "stringConstant";
  } else if(lexeme.length() > 0 && lexeme[0] >= '0' && lexeme[0] <= '9') {
    return "integerConstant";
  } else if(lexeme.length() > 0) {
    return "identifier";
  }
  return "";
}

deque<Token> Tokenizer::tokenize(string& path, string& fileName) {
  ifstream infile{path};
  string line;
  deque<Token> tokens{};

  string newTokenFileName = fileName.substr(0,fileName.length() - 5) + "_Out_T.xml";

  bool read = true;
  while(getline(infile, line)) {
    string buffer = "";

    // Filter out comments
    for(size_t i = 0; i < line.length(); i++) {
      cnt:;
      if(line[i] == '/' && (i + 1) < line.length() && line[i+1] == '/') {
        i = line.length();
      } else if(line[i] == '/' && (i + 1) < line.length() && line[i+1] == '*') {
        read = false;
        i += 2;
        goto cnt;
      } else if(line[i] == '*' && (i + 1) < line.length() && line[i+1] == '/') {
        read = true;
        i += 2;
        goto cnt;
      }

      if(read && (i < line.length())) {
        buffer += line[i];
      }
    }

    //Take the line word by word taking into account symbols can appear anywhere
    //outside strings
    for(size_t i = 0; i < buffer.length(); i++) {
      string lexeme = "";
      while((buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\f' ||
        buffer[i] == '\n' || buffer[i] == '\r' || buffer[i] == '\v')
        && i < buffer.length()) {
        i++;
      }

      if(i < line.length() && buffer[i] == '"') {
        lexeme += buffer[i];
        i++;
        while(buffer[i] != '"' && i < buffer.length()) {
          lexeme += buffer[i];
          i++;
        }
        lexeme += buffer[i];
        i++;
      } else {
        while(buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '\f' &&
          buffer[i] != '\n' && buffer[i] != '\r' && buffer[i] != '\v' &&
          i < buffer.length() && buffer[i] != '{' && buffer[i] != '}' && buffer[i] != '(' &&
          buffer[i] != ')' && buffer[i] != '[' && buffer[i] != ']' && buffer[i] != '.' &&
          buffer[i] != ',' && buffer[i] != ';' && buffer[i] != '+' && buffer[i] != '-' &&
          buffer[i] != '*' && buffer[i] != '/' && buffer[i] != '&' && buffer[i] != '|' &&
          buffer[i] != '<' && buffer[i] != '>' && buffer[i] != '=' && buffer[i] != '~') {


          lexeme += buffer[i];
          i++;
        }
      }

      //Add tokens to list of tokens
      string type = identifyLexeme(lexeme);

      if(type == "stringConstant") {
        tokens.push_back(Token{lexeme.substr(1, lexeme.length() - 2), type});
      } else if(type.length() > 0) {
        tokens.push_back(Token{lexeme, type});
      }
      if(buffer[i] == '{' || buffer[i] == '}' || buffer[i] == '(' ||
        buffer[i] == ')' || buffer[i] == '[' || buffer[i] == ']' || buffer[i] == '.' ||
        buffer[i] == ',' || buffer[i] == ';' || buffer[i] == '+' || buffer[i] == '-' ||
        buffer[i] == '*' || buffer[i] == '/' || buffer[i] == '&' || buffer[i] == '|' ||
        buffer[i] == '<' || buffer[i] == '>' || buffer[i] == '=' || buffer[i] == '~') {

        string lex = "";

        if(buffer[i] == '<') {
          lex = "&lt;";
        } else if(buffer[i] == '>') {
          lex = "&gt;";
        } else if(buffer[i] == '&') {
          lex = "&amp;";
        } else {
          lex += buffer[i];
        }
        tokens.push_back(Token{lex, "symbol"});
      }
    }
  }

  return tokens;
}
