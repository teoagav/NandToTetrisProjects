#include "parser.h"
#include <fstream>

using namespace std;

vector<Instruction> Parser::parse(string& fileName) {
  ifstream infile{fileName};
  string buffer;
  vector<Instruction> instructions{};

  while(getline(infile, buffer)) {
    string bufferNoWs = "";

    //Get line without whitesace and without comments
    for(size_t i = 0; i < buffer.length(); i++) {
      if(buffer[i] == '/' && buffer.length() > i+1 && buffer[i+1] == '/') {
        i = buffer.length();
      } else if (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '\n' && buffer[i] != '\r') {
        bufferNoWs += buffer[i];
      }
    }

    //Figure out what kind of command the line is
    if(bufferNoWs.length() > 0) {
      Instruction instruction{};
      if(bufferNoWs[0] == 64) {
        instruction.setType(Instruction::A_TYPE);
        instruction.setLeftHandSide(bufferNoWs.substr(1, bufferNoWs.length() - 1));
      } else if(bufferNoWs[0] == '(') {
        instruction.setType(Instruction::LABEL);
        instruction.setLeftHandSide(bufferNoWs.substr(1, bufferNoWs.length() - 2));
      } else {
        instruction.setType(Instruction::C_TYPE);
        size_t indexOfEqual = 0;
        size_t indexOfSemi = bufferNoWs.length();
        bool equalFound = false;
        bool semiFound = false;
        for(size_t i = 0; i < bufferNoWs.length(); i++) {
          if(bufferNoWs[i] == ';') {
            semiFound = true;
            indexOfSemi = i;
          } else if(bufferNoWs[i] == '=') {
            equalFound = true;
            indexOfEqual = i;
          }
        }

        instruction.setLeftHandSide(bufferNoWs.substr(0, indexOfEqual));
        if(equalFound) { indexOfEqual++; }
        instruction.setRightHandSide(bufferNoWs.substr(indexOfEqual, indexOfSemi - indexOfEqual));
        if(semiFound) { indexOfSemi++; }
        instruction.setJump(bufferNoWs.substr(indexOfSemi, bufferNoWs.length() - indexOfSemi));
      }
      instructions.push_back(instruction);
    }
  }
  infile.close();
  return instructions;
}
