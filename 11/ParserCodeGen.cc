#include "ParserCodeGen.h"
#include "SymbolTableItem.h"
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

map<string, SymbolTableItem> globalScopeSymbolTable;
map<string, SymbolTableItem> functionScopeSymbolTable;
map<string, string> functionSymbolTable{};
string className;
size_t whiles = 0;
size_t ifs = 0;

void parseClassVarDec(deque<Token>& tokens, ofstream& outfile, size_t& fields, size_t& statics) {
  outfile << "\n// begin class var dec" << endl;

  string kind = tokens[0].getLexeme();
  string type = tokens[1].getLexeme();
  if(kind == "field") {
    globalScopeSymbolTable[tokens[2].getLexeme()] = SymbolTableItem(type, kind, fields);
    fields++;
  }
  if(kind == "static") {
    globalScopeSymbolTable[tokens[2].getLexeme()] = SymbolTableItem(type, kind, statics);
    statics++;
  }
  tokens.pop_front(); //static or field
  tokens.pop_front(); //type
  tokens.pop_front(); //varname

  while(tokens[0].getLexeme() == ",") {
    if(kind == "field") {
      globalScopeSymbolTable[tokens[1].getLexeme()] = SymbolTableItem(type, kind, fields);
      fields++;
    }
    if(kind == "static") {
      globalScopeSymbolTable[tokens[1].getLexeme()] = SymbolTableItem(type, kind, statics);
      statics++;
    }
    tokens.pop_front(); //,
    tokens.pop_front(); //varname
  }

  tokens.pop_front(); // ;
  outfile << "// end class var dec\n" << endl;
}

void parseVarDec(deque<Token>& tokens, ofstream& outfile, size_t& fields) {
  outfile << "\n// begin var dec" << endl;
  string type = tokens[1].getLexeme();
  functionScopeSymbolTable[tokens[2].getLexeme()] = SymbolTableItem(type, "local", fields);
  fields++;
  tokens.pop_front(); //var
  tokens.pop_front(); //type
  tokens.pop_front(); //varname

  while(tokens[0].getLexeme() == ",") {
    functionScopeSymbolTable[tokens[1].getLexeme()] = SymbolTableItem(type, "local", fields);
    fields++;
    tokens.pop_front(); //,
    tokens.pop_front(); //varname
  }
  tokens.pop_front(); // ;
  outfile << "// end var dec\n" << endl;
}

void parseParameterList(deque<Token>& tokens, ofstream& outfile, string& functionType) {
  outfile << "\n// begin parameter list" << endl;
  size_t number = 0;
  if(functionType == "method") {
    number++;
  }
  if(tokens[0].getLexeme() != ")") {
    functionScopeSymbolTable[tokens[1].getLexeme()] = SymbolTableItem(tokens[0].getLexeme(), "argument", number);
    number++;
    tokens.pop_front(); // type
    tokens.pop_front(); // varname
    while(tokens[0].getLexeme() == ",") {
      functionScopeSymbolTable[tokens[2].getLexeme()] = SymbolTableItem(tokens[1].getLexeme(), "argument", number);
      number++;
      tokens.pop_front(); //,
      tokens.pop_front(); //type
      tokens.pop_front(); //varname
    }
  }
  outfile << "// end parameter list\n" << endl;
}

void parseExpression(deque<Token>& tokens, ofstream& outfile);
size_t parseExpressionList(deque<Token>& tokens, ofstream& outfile);

void parseTerm(deque<Token>& tokens, ofstream& outfile) {
  string previousLex = tokens[0].getLexeme();
  string previousType = tokens[0].getType();
  outfile << "\n// begin term " << previousType << " " << previousLex << endl;
  tokens.pop_front(); // many different things

  if(previousType == "stringConstant") {
    outfile << "push constant " << previousLex.length() << endl;
    outfile << "call String.new 1" << endl;
    for(size_t i = 0; i < previousLex.length(); i++) {
      outfile << "push constant " << int(previousLex[i]) << endl;
      outfile << "call String.appendChar 2" << endl;
    }
  } else if(previousType == "integerConstant") {
    outfile << "push constant " << previousLex << endl;
  } else if(previousType == "keyword") {
    if(previousLex == "true") {
      outfile << "push constant 1" << endl;
      outfile << "neg" << endl;
    } else if(previousLex == "false") {
      outfile << "push constant 0" << endl;
    } else if(previousLex == "null") {
      outfile << "push constant 0" << endl;
    } else if(previousLex == "this") {
      outfile << "push pointer 0" << endl;
    }
  } else if(functionScopeSymbolTable.count(previousLex) > 0) {
    outfile << "push " << functionScopeSymbolTable[previousLex].getKind() << " ";
    outfile << functionScopeSymbolTable[previousLex].getNumber() << endl;
  } else if(globalScopeSymbolTable.count(previousLex) > 0) {
    if(globalScopeSymbolTable[previousLex].getKind() == "static") {
      outfile << "push static " << globalScopeSymbolTable[previousLex].getNumber() << endl;
    } else { // field
      outfile << "push pointer 0" << endl;
      outfile << "push constant " << globalScopeSymbolTable[previousLex].getNumber() << endl;
      outfile << "add" << endl;
      outfile << "pop pointer 1" << endl;
      outfile << "push that 0" << endl;
    }
  }

  if(tokens[0].getLexeme() == "[") { // varname[expression]
    tokens.pop_front(); // [
    parseExpression(tokens, outfile);
    outfile << "add" << endl;
    outfile << "pop pointer 1" << endl;
    outfile << "push that 0" << endl;
    tokens.pop_front(); // ]
  } else if (previousLex == "(") { // (expression)
    parseExpression(tokens, outfile);
    tokens.pop_front(); // )
  } else if(previousLex == "-" || previousLex == "~") { // unaryop term
    parseTerm(tokens, outfile);
    if(previousLex == "-") {
      outfile << "neg" << endl;
    } else {
      outfile << "not" << endl;
    }
  } else if(tokens[0].getLexeme() == "(" || tokens[0].getLexeme() == ".") { // subroutineCall
    size_t argCount = 0;
    string functionName = previousLex;

    if(tokens[0].getLexeme() == ".") {
      if(functionScopeSymbolTable.count(functionName) > 0) {
        //outfile << "push " << functionScopeSymbolTable[functionName].getKind() << " ";
        //outfile << functionScopeSymbolTable[functionName].getNumber() << endl; // push the object reference
        argCount++;
        functionName = functionScopeSymbolTable[functionName].getType();
      } else if(globalScopeSymbolTable.count(functionName) > 0) {
        //outfile << "push pointer 0" << endl;
        //outfile << "push constant " << globalScopeSymbolTable[functionName].getNumber() << endl;
        //outfile << "add" << endl;
        //outfile << "pop pointer 1" << endl;
        //outfile << "push that 0" << endl;
        argCount++;
        functionName = globalScopeSymbolTable[functionName].getType();
      }
      functionName += "." + tokens[1].getLexeme();
      tokens.pop_front(); // .
      tokens.pop_front(); // subroutinename
    } else {
      if(functionSymbolTable[functionName] == "method") {
        outfile << "push pointer 0" << endl;
        argCount++;
      }
      functionName = className + "." + functionName;
    }
    tokens.pop_front(); // (

    argCount += parseExpressionList(tokens, outfile);
    outfile << "call " << functionName << " " << argCount << endl; // agav
    tokens.pop_front(); // )
  }
  outfile << "// end term\n" << endl;
}

void parseExpression(deque<Token>& tokens, ofstream& outfile) {
  outfile << "\n// begin expression" << endl;
  parseTerm(tokens, outfile);
  while(tokens[0].getLexeme() == "+" || tokens[0].getLexeme() == "-" ||
    tokens[0].getLexeme() == "*" || tokens[0].getLexeme() == "/" ||
    tokens[0].getLexeme() == "&amp;" || tokens[0].getLexeme() == "|" ||
    tokens[0].getLexeme() == "&lt;" || tokens[0].getLexeme() == "&gt;" ||
    tokens[0].getLexeme() == "=") {

    string operation = tokens[0].getLexeme();
    tokens.pop_front(); // op
    parseTerm(tokens, outfile);
    if(operation == "+") {
      outfile << "add" << endl;
    } else if(operation == "-") {
      outfile << "sub" << endl;
    } else if(operation == "*") {
      outfile << "call Math.multiply 2" << endl;
    } else if(operation == "/") {
      outfile << "call Math.divide 2" << endl;
    } else if(operation == "&amp;") {
      outfile << "and" << endl;
    } else if(operation == "|") {
      outfile << "or" << endl;
    } else if(operation == "&lt;") {
      outfile << "lt" << endl;
    } else if(operation == "&gt;") {
      outfile << "gt" << endl;
    } else if(operation == "=") {
      outfile << "eq" << endl;
    }
  }
  outfile << "// end expression\n" << endl;
}

size_t parseExpressionList(deque<Token>& tokens, ofstream& outfile) {
  outfile << "\n// begin expression list" << endl;
  size_t count = 0;
  if(tokens[0].getLexeme() != ")") {
    parseExpression(tokens, outfile);
    count++;
    while(tokens[0].getLexeme() == ",") {
      tokens.pop_front(); // ,
      parseExpression(tokens, outfile);
      count++;
    }
  }
  outfile << "// end expression list\n" << endl;
  return count;
}

void parseStatements(deque<Token>& tokens, ofstream& outfile);

void parseIfStatement(deque<Token>& tokens, ofstream& outfile) {
  outfile << "\n// begin if" << endl;
  tokens.pop_front(); // if
  tokens.pop_front(); // (
  size_t ifCount = ifs;
  ifs++;

  parseExpression(tokens, outfile);
  outfile << "not" << endl;
  outfile << "if-goto endif_" << ifCount << endl;

  tokens.pop_front(); // )
  tokens.pop_front(); // {
  parseStatements(tokens, outfile);
  tokens.pop_front(); // }


  outfile << "goto endelse_" << ifCount << endl;
  outfile << "label endif_" << ifCount << endl;
  if(tokens[0].getLexeme() == "else") {
    tokens.pop_front(); // else
    tokens.pop_front(); // {

    parseStatements(tokens, outfile);

    tokens.pop_front(); // }
  }
  outfile << "label endelse_" << ifCount << endl;
  outfile << "// end if\n" << endl;
}

void parseLetStatement(deque<Token>& tokens, ofstream& outfile) {
  outfile << "\n// begin let" << endl;
  string varName = tokens[1].getLexeme();
  tokens.pop_front(); // let
  tokens.pop_front(); // varname
  bool addressing = false;

  if(tokens[0].getLexeme() == "[") {
    tokens.pop_front(); // [
    parseExpression(tokens, outfile);
    tokens.pop_front(); // ]
    addressing = true;

    if(functionScopeSymbolTable.count(varName) > 0) {
      outfile << "push " << functionScopeSymbolTable[varName].getKind() << " ";
      outfile << functionScopeSymbolTable[varName].getNumber() << endl;
    } else if(globalScopeSymbolTable.count(varName) > 0) {
      if(globalScopeSymbolTable[varName].getKind() == "static") {
        outfile << "push static " << globalScopeSymbolTable[varName].getNumber() << endl;
      } else { // field
        outfile << "push pointer 0" << endl;
        outfile << "push constant " << globalScopeSymbolTable[varName].getNumber() << endl;
        outfile << "add" << endl;
        outfile << "pop pointer 1" << endl;
        outfile << "push that 0" << endl;
      }
    }
    outfile << "add" << endl;
  }
  tokens.pop_front(); // =

  parseExpression(tokens, outfile);
  tokens.pop_front(); // ;

  if(addressing) {
    outfile << "pop temp 1" << endl;
    outfile << "pop pointer 1" << endl;
    outfile << "push temp 1" << endl;
    outfile << "pop that 0" << endl;
  } else if(functionScopeSymbolTable.count(varName) > 0) {
    outfile << "pop " << functionScopeSymbolTable[varName].getKind() << " ";
    outfile << functionScopeSymbolTable[varName].getNumber() << endl;

  } else if(globalScopeSymbolTable.count(varName) > 0) {
    if(globalScopeSymbolTable[varName].getKind() == "static") {
      outfile << "pop static " << globalScopeSymbolTable[varName].getNumber() << endl;
    } else { // field
      outfile << "push pointer 0" << endl;
      outfile << "push constant " << globalScopeSymbolTable[varName].getNumber() << endl;
      outfile << "add" << endl;
      outfile << "pop pointer 1" << endl;
      outfile << "pop that 0" << endl;
    }
  }
  outfile << "// end let\n" << endl;
}

void parseWhileStatement(deque<Token>& tokens, ofstream& outfile) {
  outfile << "\n// begin while" << endl;
  tokens.pop_front(); // while
  tokens.pop_front(); // (
  size_t whileCount = whiles;
  whiles++;

  outfile << "label while_" << whileCount << endl;
  parseExpression(tokens, outfile);
  outfile << "not" << endl;
  outfile << "if-goto endwhile_" << whileCount << endl;

  tokens.pop_front(); // )
  tokens.pop_front(); // {
  parseStatements(tokens, outfile);
  tokens.pop_front(); // }

  outfile << "goto while_" << whileCount << endl;
  outfile << "label endwhile_" << whileCount << endl;
  outfile << "// end while\n" << endl;
}

void parseDoStatement(deque<Token>& tokens, ofstream& outfile) {
  outfile << "\n// begin do" << endl;
  string functionName = tokens[1].getLexeme();
  size_t argCount = 0;

  tokens.pop_front(); // do
  tokens.pop_front(); // classname or varname or subroutinename

  if(tokens[0].getLexeme() == ".") {
    if(functionScopeSymbolTable.count(functionName) > 0) {
      outfile << "push " << functionScopeSymbolTable[functionName].getKind() << " ";
      outfile << functionScopeSymbolTable[functionName].getNumber() << endl; // push the object reference
      argCount++;
      functionName = functionScopeSymbolTable[functionName].getType();
    } else if(globalScopeSymbolTable.count(functionName) > 0) {
      outfile << "push pointer 0" << endl;
      outfile << "push constant " << globalScopeSymbolTable[functionName].getNumber() << endl;
      outfile << "add" << endl;
      outfile << "pop pointer 1" << endl;
      outfile << "push that 0" << endl;
      argCount++;
      functionName = globalScopeSymbolTable[functionName].getType();
    }
    functionName += "." + tokens[1].getLexeme();
    tokens.pop_front(); // .
    tokens.pop_front(); // subroutinename
  } else {
    if(functionSymbolTable[functionName] == "method") {
      outfile << "push pointer 0" << endl;
      argCount++;
    }
    functionName = className + "." + functionName;
  }
  tokens.pop_front(); // (

  argCount += parseExpressionList(tokens, outfile);
  outfile << "call " << functionName << " " << argCount << endl; // agav
  outfile << "pop temp 0" << endl; // pop return value into ignored area

  tokens.pop_front(); // )
  tokens.pop_front(); // ;
  outfile << "// end do\n" << endl;
}

void parseReturnStatement(deque<Token>& tokens, ofstream& outfile) {
  outfile << "\n// begin return" << endl;
  tokens.pop_front(); // return

  if(tokens[0].getLexeme() != ";") {
    parseExpression(tokens, outfile);
  } else {
    outfile << "push constant 0" << endl;
  }
  tokens.pop_front(); // ;
  outfile << "return" << endl;
  outfile << "// end return\n" << endl;
}

void parseStatements(deque<Token>& tokens, ofstream& outfile) {
  outfile << "\n// begin statements" << endl;
  while(tokens[0].getLexeme() == "if" || tokens[0].getLexeme() == "let" ||
    tokens[0].getLexeme() == "while" || tokens[0].getLexeme() == "do" ||
    tokens[0].getLexeme() == "return") {

    if(tokens[0].getLexeme() == "if") {
      parseIfStatement(tokens, outfile);
    } else if(tokens[0].getLexeme() == "let") {
      parseLetStatement(tokens, outfile);
    } else if(tokens[0].getLexeme() == "while") {
      parseWhileStatement(tokens, outfile);
    } else if(tokens[0].getLexeme() == "do") {
      parseDoStatement(tokens, outfile);
    } else { // return
      parseReturnStatement(tokens, outfile);
    }
  }
  outfile << "// end statements\n" << endl;
}

void parseSubroutineBody(deque<Token>& tokens, ofstream& outfile) {
  outfile << "\n// begin subroutine body" << endl;
  tokens.pop_front(); // {
  size_t fields = 0;
  while(tokens[0].getLexeme() == "var") {
    parseVarDec(tokens, outfile, fields);
  } // varDec*
  parseStatements(tokens, outfile);

  tokens.pop_front(); // }
  outfile << "// end subroutine body\n" << endl;
}

void parseSubroutineDec(deque<Token>& tokens, ofstream& outfile, bool constructor, bool method, size_t fields) {
  outfile << "\n// begin subroutine dec" << endl;
  functionScopeSymbolTable.clear();

  string functionName = tokens[2].getLexeme();
  string functionType = tokens[0].getLexeme();

  tokens.pop_front(); // constructor or function or method
  tokens.pop_front(); // void or type
  tokens.pop_front(); // subroutineName
  tokens.pop_front(); // (
  parseParameterList(tokens, outfile, functionType);
  tokens.pop_front(); // )

  size_t localVars = 0;
  size_t i = 0;
  while(tokens[i].getLexeme() != "let" && tokens[i].getLexeme() != "if" &&
    tokens[i].getLexeme() != "while" && tokens[i].getLexeme() != "do" &&
    tokens[i].getLexeme() != "return") {
    if(tokens[i].getLexeme() == "," || tokens[i].getLexeme() == "var") {
      localVars++;
    }
    i++;
  }
  outfile << "function " << className << "." << functionName << " " << localVars << endl;
  if(constructor) {
    outfile << "push constant " << fields << endl;
    outfile << "call Memory.alloc 1" << endl;
    outfile << "pop pointer 0" << endl;
  }
  if(method) {
    outfile << "push argument 0" << endl;
    outfile << "pop pointer 0" << endl;
  }

  parseSubroutineBody(tokens, outfile); // subroutineBody
  outfile << "// end subroutine dec\n" << endl;
}

void ParserCodeGen::parseGenerate(deque<Token>& tokens, string fileName) {
  ofstream outfile{fileName.substr(0,fileName.length() - 5) + ".vm"};
  outfile << "\n// begin class" << endl;
  globalScopeSymbolTable.clear();
  functionSymbolTable.clear();
  className = tokens[1].getLexeme();

  tokens.pop_front(); // class
  tokens.pop_front(); // classname
  tokens.pop_front(); // {

  size_t fields = 0;
  size_t statics = 0;
  while(tokens[0].getLexeme() == "static" || tokens[0].getLexeme() == "field") {
    parseClassVarDec(tokens, outfile, fields, statics);
  } // classVarDec*

  for(size_t i = 0; i < tokens.size(); i++) {
    if(tokens[i].getLexeme() == "function" || tokens[i].getLexeme() == "method" ||
      tokens[i].getLexeme() == "constructor"){
      functionSymbolTable[tokens[i+2].getLexeme()] = tokens[i].getLexeme();
    }
  }

  while(tokens[0].getLexeme() == "constructor" ||
    tokens[0].getLexeme() == "function" ||
    tokens[0].getLexeme() == "method") {

    bool constructor = false;
    bool method = false;
    if(tokens[0].getLexeme() == "constructor") {
      constructor = true;
    } else if(tokens[0].getLexeme() == "method") {
      method = true;
    }

    parseSubroutineDec(tokens, outfile, constructor, method, fields);
  } // subroutineDec*

  tokens.pop_front(); // }
  outfile << "// end class\n" << endl;
  outfile.close();
}
