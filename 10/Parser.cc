#include "Parser.h"
#include <iostream>
#include <fstream>

using namespace std;

void printTabs(size_t depth, ofstream& outfile) {
  for(size_t i = 0; i < depth*2; i++) {
    outfile << ' ';
  }
}

void printLex(deque<Token>& tokens, size_t depth, ofstream& outfile) {
  printTabs(depth, outfile);
  outfile << "<" << tokens[0].getType() << "> " << tokens[0].getLexeme();
  outfile << " </" << tokens[0].getType() << ">" << endl;
}

void parseClassVarDec(deque<Token>& tokens, size_t depth, ofstream& outfile) {
  printTabs(depth, outfile);
  depth++;
  outfile << "<classVarDec>" << endl;

  printLex(tokens, depth, outfile);
  tokens.pop_front(); //static or field
  printLex(tokens, depth, outfile);
  tokens.pop_front(); //type
  printLex(tokens, depth, outfile);
  tokens.pop_front(); //varname

  while(tokens[0].getLexeme() == ",") {
    printLex(tokens, depth, outfile);
    tokens.pop_front(); //,
    printLex(tokens, depth, outfile);
    tokens.pop_front(); //varname
  }

  printLex(tokens, depth, outfile);
  tokens.pop_front(); // ;

  printTabs(--depth, outfile);
  outfile << "</classVarDec>" << endl;
}

void parseVarDec(deque<Token>& tokens, size_t depth, ofstream& outfile) {
  printTabs(depth, outfile);
  depth++;
  outfile << "<varDec>" << endl;

  printLex(tokens, depth, outfile);
  tokens.pop_front(); //var
  printLex(tokens, depth, outfile);
  tokens.pop_front(); //type
  printLex(tokens, depth, outfile);
  tokens.pop_front(); //varname

  while(tokens[0].getLexeme() == ",") {
    printLex(tokens, depth, outfile);
    tokens.pop_front(); //,
    printLex(tokens, depth, outfile);
    tokens.pop_front(); //varname
  }
  printLex(tokens, depth, outfile);
  tokens.pop_front(); // ;

  printTabs(--depth, outfile);
  outfile << "</varDec>" << endl;
}

void parseParameterList(deque<Token>& tokens, size_t depth, ofstream& outfile) {
  printTabs(depth, outfile);
  depth++;
  outfile << "<parameterList>" << endl;

  if(tokens[0].getLexeme() != ")") {
    printLex(tokens, depth, outfile);
    tokens.pop_front(); // type
    printLex(tokens, depth, outfile);
    tokens.pop_front(); // varname
    while(tokens[0].getLexeme() == ",") {
      printLex(tokens, depth, outfile);
      tokens.pop_front(); //,
      printLex(tokens, depth, outfile);
      tokens.pop_front(); //type
      printLex(tokens, depth, outfile);
      tokens.pop_front(); //varname
    }
  }

  printTabs(--depth, outfile);
  outfile << "</parameterList>" << endl;
}

void parseExpression(deque<Token>& tokens, size_t depth, ofstream& outfile);
void parseExpressionList(deque<Token>& tokens, size_t depth, ofstream& outfile);

void parseTerm(deque<Token>& tokens, size_t depth, ofstream& outfile) {
  printTabs(depth, outfile);
  depth++;
  outfile << "<term>" << endl;

  string previousLex = tokens[0].getLexeme();
  printLex(tokens, depth, outfile);
  tokens.pop_front(); // many different things
  if(tokens[0].getLexeme() == "[") { // varname[expression]
    printLex(tokens, depth, outfile);
    tokens.pop_front(); // [

    parseExpression(tokens, depth, outfile);

    printLex(tokens, depth, outfile);
    tokens.pop_front(); // ]
  } else if (previousLex == "(") { // (expression)
    parseExpression(tokens, depth, outfile);

    printLex(tokens, depth, outfile);
    tokens.pop_front(); // )
  } else if(previousLex == "-" || previousLex == "~") { // unaryop term
    parseTerm(tokens, depth, outfile);
  } else if(tokens[0].getLexeme() == "(" || tokens[0].getLexeme() == ".") { // subroutineCall
    if(tokens[0].getLexeme() == ".") {
      printLex(tokens, depth, outfile);
      tokens.pop_front(); // .
      printLex(tokens, depth, outfile);
      tokens.pop_front(); // subroutinename
    }
    printLex(tokens, depth, outfile);
    tokens.pop_front(); // (

    parseExpressionList(tokens, depth, outfile);

    printLex(tokens, depth, outfile);
    tokens.pop_front(); // )
  }

  printTabs(--depth, outfile);
  outfile << "</term>" << endl;
}

void parseExpression(deque<Token>& tokens, size_t depth, ofstream& outfile) {
  printTabs(depth, outfile);
  depth++;
  outfile << "<expression>" << endl;

  parseTerm(tokens, depth, outfile);
  while(tokens[0].getLexeme() == "+" || tokens[0].getLexeme() == "-" ||
    tokens[0].getLexeme() == "*" || tokens[0].getLexeme() == "/" ||
    tokens[0].getLexeme() == "&amp;" || tokens[0].getLexeme() == "|" ||
    tokens[0].getLexeme() == "&lt;" || tokens[0].getLexeme() == "&gt;" ||
    tokens[0].getLexeme() == "=") {

    printLex(tokens, depth, outfile);
    tokens.pop_front(); // op
    parseTerm(tokens, depth, outfile);
  }

  printTabs(--depth, outfile);
  outfile << "</expression>" << endl;
}

void parseExpressionList(deque<Token>& tokens, size_t depth, ofstream& outfile) {
  printTabs(depth, outfile);
  depth++;
  outfile << "<expressionList>" << endl;

  if(tokens[0].getLexeme() != ")") {
    parseExpression(tokens, depth, outfile);
    while(tokens[0].getLexeme() == ",") {
      printLex(tokens, depth, outfile);
      tokens.pop_front(); // ,
      parseExpression(tokens, depth, outfile);
    }
  }

  printTabs(--depth, outfile);
  outfile << "</expressionList>" << endl;
}

void parseStatements(deque<Token>& tokens, size_t depth, ofstream& outfile);

void parseIfStatement(deque<Token>& tokens, size_t depth, ofstream& outfile) {
  printTabs(depth, outfile);
  depth++;
  outfile << "<ifStatement>" << endl;

  printLex(tokens, depth, outfile);
  tokens.pop_front(); // if
  printLex(tokens, depth, outfile);
  tokens.pop_front(); // (

  parseExpression(tokens, depth, outfile);

  printLex(tokens, depth, outfile);
  tokens.pop_front(); // )
  printLex(tokens, depth, outfile);
  tokens.pop_front(); // {

  parseStatements(tokens, depth, outfile);

  printLex(tokens, depth, outfile);
  tokens.pop_front(); // }

  if(tokens[0].getLexeme() == "else") {
    printLex(tokens, depth, outfile);
    tokens.pop_front(); // else
    printLex(tokens, depth, outfile);
    tokens.pop_front(); // {

    parseStatements(tokens, depth, outfile);

    printLex(tokens, depth, outfile);
    tokens.pop_front(); // }
  }

  printTabs(--depth, outfile);
  outfile << "</ifStatement>" << endl;
}

void parseLetStatement(deque<Token>& tokens, size_t depth, ofstream& outfile) {
  printTabs(depth, outfile);
  depth++;
  outfile << "<letStatement>" << endl;

  printLex(tokens, depth, outfile);
  tokens.pop_front(); // let
  printLex(tokens, depth, outfile);
  tokens.pop_front(); // varname
  if(tokens[0].getLexeme() == "[") {
    printLex(tokens, depth, outfile);
    tokens.pop_front(); // [

    parseExpression(tokens, depth, outfile);

    printLex(tokens, depth, outfile);
    tokens.pop_front(); // ]
  }
  printLex(tokens, depth, outfile);
  tokens.pop_front(); // =

  parseExpression(tokens, depth, outfile);
  printLex(tokens, depth, outfile);
  tokens.pop_front(); // ;

  printTabs(--depth, outfile);
  outfile << "</letStatement>" << endl;
}

void parseWhileStatement(deque<Token>& tokens, size_t depth, ofstream& outfile) {
  printTabs(depth, outfile);
  depth++;
  outfile << "<whileStatement>" << endl;

  printLex(tokens, depth, outfile);
  tokens.pop_front(); // while
  printLex(tokens, depth, outfile);
  tokens.pop_front(); // (

  parseExpression(tokens, depth, outfile);

  printLex(tokens, depth, outfile);
  tokens.pop_front(); // )
  printLex(tokens, depth, outfile);
  tokens.pop_front(); // {

  parseStatements(tokens, depth, outfile);

  printLex(tokens, depth, outfile);
  tokens.pop_front(); // }

  printTabs(--depth, outfile);
  outfile << "</whileStatement>" << endl;
}

void parseDoStatement(deque<Token>& tokens, size_t depth, ofstream& outfile) {
  printTabs(depth, outfile);
  depth++;
  outfile << "<doStatement>" << endl;

  printLex(tokens, depth, outfile);
  tokens.pop_front(); // do

  printLex(tokens, depth, outfile);
  tokens.pop_front(); // classname or varname or subroutinename
  if(tokens[0].getLexeme() == ".") {
    printLex(tokens, depth, outfile);
    tokens.pop_front(); // .
    printLex(tokens, depth, outfile);
    tokens.pop_front(); // subroutinename
  }
  printLex(tokens, depth, outfile);
  tokens.pop_front(); // (

  parseExpressionList(tokens, depth, outfile);

  printLex(tokens, depth, outfile);
  tokens.pop_front(); // )
  printLex(tokens, depth, outfile);
  tokens.pop_front(); // ;

  printTabs(--depth, outfile);
  outfile << "</doStatement>" << endl;
}

void parseReturnStatement(deque<Token>& tokens, size_t depth, ofstream& outfile) {
  printTabs(depth, outfile);
  depth++;
  outfile << "<returnStatement>" << endl;

  printLex(tokens, depth, outfile);
  tokens.pop_front(); // return

  if(tokens[0].getLexeme() != ";") {
    parseExpression(tokens, depth, outfile);
  }
  printLex(tokens, depth, outfile);
  tokens.pop_front(); // ;

  printTabs(--depth, outfile);
  outfile << "</returnStatement>" << endl;
}

void parseStatements(deque<Token>& tokens, size_t depth, ofstream& outfile) {
  printTabs(depth, outfile);
  depth++;
  outfile << "<statements>" << endl;

  while(tokens[0].getLexeme() == "if" || tokens[0].getLexeme() == "let" ||
    tokens[0].getLexeme() == "while" || tokens[0].getLexeme() == "do" ||
    tokens[0].getLexeme() == "return") {

    if(tokens[0].getLexeme() == "if") {
      parseIfStatement(tokens, depth, outfile);
    } else if(tokens[0].getLexeme() == "let") {
      parseLetStatement(tokens, depth, outfile);
    } else if(tokens[0].getLexeme() == "while") {
      parseWhileStatement(tokens, depth, outfile);
    } else if(tokens[0].getLexeme() == "do") {
      parseDoStatement(tokens, depth, outfile);
    } else { // return
      parseReturnStatement(tokens, depth, outfile);
    }
  }

  printTabs(--depth, outfile);
  outfile << "</statements>" << endl;
}

void parseSubroutineBody(deque<Token>& tokens, size_t depth, ofstream& outfile) {
  printTabs(depth, outfile);
  depth++;
  outfile << "<subroutineBody>" << endl;

  printLex(tokens, depth, outfile);
  tokens.pop_front(); // {
  while(tokens[0].getLexeme() == "var") {
    parseVarDec(tokens, depth, outfile);
  } // varDec*
  parseStatements(tokens, depth, outfile);

  printLex(tokens, depth, outfile);
  tokens.pop_front(); // }

  printTabs(--depth, outfile);
  outfile << "</subroutineBody>" << endl;
}

void parseSubroutineDec(deque<Token>& tokens, size_t depth, ofstream& outfile) {
  printTabs(depth, outfile);
  depth++;
  outfile << "<subroutineDec>" << endl;

  printLex(tokens, depth, outfile);
  tokens.pop_front(); // constructor or function or method
  printLex(tokens, depth, outfile);
  tokens.pop_front(); // void or type
  printLex(tokens, depth, outfile);
  tokens.pop_front(); // subroutineName
  printLex(tokens, depth, outfile);
  tokens.pop_front(); // (

  parseParameterList(tokens, depth, outfile);

  printLex(tokens, depth, outfile);
  tokens.pop_front(); // )

  parseSubroutineBody(tokens, depth, outfile); // subroutineBody
  printTabs(--depth, outfile);
  outfile << "</subroutineDec>" << endl;
}

void Parser::parse(deque<Token>& tokens, string fileName) {
  ofstream outfile{fileName.substr(0,fileName.length() - 5) + "_Out.xml"};
  outfile << "<class>" << endl;

  printLex(tokens, 1, outfile);
  tokens.pop_front(); // class
  printLex(tokens, 1, outfile);
  tokens.pop_front(); // classname
  printLex(tokens, 1, outfile);
  tokens.pop_front(); // {

  while(tokens[0].getLexeme() == "static" || tokens[0].getLexeme() == "field") {
    parseClassVarDec(tokens, 1, outfile);
  } // classVarDec*
  while(tokens[0].getLexeme() == "constructor" ||
    tokens[0].getLexeme() == "function" ||
    tokens[0].getLexeme() == "method") {

    parseSubroutineDec(tokens, 1, outfile);
  } // subroutineDec*

  printLex(tokens, 1, outfile);
  tokens.pop_front(); // }

  outfile << "</class>" << endl;
  outfile.close();
}
