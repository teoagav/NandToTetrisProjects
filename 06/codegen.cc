#include "codegen.h"
#include <iostream>
#include <fstream>

const string outputFile = "Prog.hack";

using namespace std;

string integerToBinaryString(size_t integer) {
  string binary = "";
  while (integer != 0) {
    if(integer % 2 == 1) {
      binary = "1" + binary;
    } else {
      binary = "0" + binary;
    }
    integer /= 2;
  }

  while(binary.length() < 15) {
    binary = "0" + binary;
  }

  return binary;
}

string decimalStringToBinaryString(string decimal) {
  size_t integer = 0;
  size_t digit = 1;
  for(size_t i = decimal.length() - 1; i != static_cast<size_t>(-1); i--) {
    integer += (decimal[i] - 48) * digit;
    digit *= 10;
  }
  return integerToBinaryString(integer);
}

string generateABit(string instructionPart) {
  for(size_t i = 0; i < instructionPart.size(); i++) {
    if(instructionPart[i] == 'M') {
      return "1";
    }
  }
  return "0";
}

string generateCBits(string instructionPart) {
  if(instructionPart == "0") {
    return "101010";
  } else if(instructionPart == "1") {
    return "111111";
  } else if(instructionPart == "-1") {
    return "111010";
  } else if(instructionPart == "D") {
    return "001100";
  } else if(instructionPart == "A" || instructionPart == "M") {
    return "110000";
  } else if(instructionPart == "!D") {
    return "001101";
  } else if(instructionPart == "!A" || instructionPart == "!M") {
    return "110001";
  } else if(instructionPart == "-D") {
    return "001111";
  } else if(instructionPart == "-A" || instructionPart == "-M") {
    return "110011";
  } else if(instructionPart == "D+1") {
    return "011111";
  } else if(instructionPart == "A+1" || instructionPart == "M+1") {
    return "110111";
  } else if(instructionPart == "D-1") {
    return "001110";
  } else if(instructionPart == "A-1" || instructionPart == "M-1") {
    return "110010";
  } else if(instructionPart == "D+A" || instructionPart == "D+M") {
    return "000010";
  } else if(instructionPart == "D-A" || instructionPart == "D-M") {
    return "010011";
  } else if(instructionPart == "A-D" || instructionPart == "M-D") {
    return "000111";
  } else if(instructionPart == "D&A" || instructionPart == "D&M") {
    return "000000";
  } else if(instructionPart == "D|A" || instructionPart == "D|M") {
    return "010101";
  }
  return "ERROR";
}

string generateDBits(string instructionPart) {
  bool a = false;
  bool m = false;
  bool d = false;
  for(size_t i = 0; i < instructionPart.size(); i++) {
    if(instructionPart[i] == 'A') {
      a = true;
    } else if(instructionPart[i] == 'M') {
      m = true;
    } else if(instructionPart[i] == 'D'){
      d = true;
    }
  }
  string dBits = "";
  if(a) {dBits += "1";} else{dBits += "0";}
  if(d) {dBits += "1";} else{dBits += "0";}
  if(m) {dBits += "1";} else{dBits += "0";}
  return dBits;
}

string generateJBits(string instructionPart) {
  if(instructionPart == "JGT") {
    return "001";
  } else if(instructionPart == "JEQ") {
    return "010";
  } else if(instructionPart == "JGE") {
    return "011";
  } else if(instructionPart == "JLT") {
    return "100";
  } else if(instructionPart == "JNE") {
    return "101";
  } else if(instructionPart == "JLE") {
    return "110";
  } else if(instructionPart == "JMP") {
    return "111";
  }
  return "000";
}

void CodeGenerator::generate(vector<Instruction>& lines) {
  map<string, size_t> symbolTable{};
  size_t programCounter = 0;

  //Fill the symbol table on the first pass
  for(auto& instruction : lines) {
    if(instruction.getType() != Instruction::LABEL) {
      programCounter++;
    } else {
      symbolTable[instruction.getLeftHandSide()] = programCounter;
    }
  }

  ofstream outfile ("Prog.hack");

  //Generate code on second pass
  size_t variableAddress = 16;
  for(auto& instruction : lines) {
    string output = "";
    if(instruction.getType() == Instruction::A_TYPE) {
      output += "0";
      if(symbolTable.count(instruction.getLeftHandSide()) > 0) {
        output += integerToBinaryString(symbolTable[instruction.getLeftHandSide()]);
      } else if(instruction.getLeftHandSide().length() <= 3 && instruction.getLeftHandSide()[0] == 'R')  {
        output += decimalStringToBinaryString(instruction.getLeftHandSide().substr(1, instruction.getLeftHandSide().length()));
      } else if(instruction.getLeftHandSide() == "SP") {
        output += "000000000000000";
      } else if(instruction.getLeftHandSide() == "LCL") {
        output += "000000000000001";
      } else if(instruction.getLeftHandSide() == "ARG") {
        output += "000000000000010";
      } else if(instruction.getLeftHandSide() == "THIS") {
        output += "000000000000011";
      } else if(instruction.getLeftHandSide() == "THAT") {
        output += "000000000000100";
      } else if(instruction.getLeftHandSide() == "SCREEN") {
        output += "100000000000000";
      } else if(instruction.getLeftHandSide() == "KBD") {
        output += "110000000000000";
      } else if(instruction.getLeftHandSide()[0] >= '0' && instruction.getLeftHandSide()[0] <= '9') {
        output += decimalStringToBinaryString(instruction.getLeftHandSide());
      } else {
        output += integerToBinaryString(variableAddress);
        symbolTable[instruction.getLeftHandSide()] = variableAddress;
        variableAddress++;
      }
      outfile << output << endl;
    } else if(instruction.getType() == Instruction::C_TYPE) {
      output += "111";
      output += generateABit(instruction.getRightHandSide());
      output += generateCBits(instruction.getRightHandSide());
      output += generateDBits(instruction.getLeftHandSide());
      output += generateJBits(instruction.getJump());
      outfile << output << endl;
    }
  }
  outfile.close();

}
