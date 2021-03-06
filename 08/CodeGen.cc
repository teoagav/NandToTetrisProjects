#include "CodeGen.h"
#include <iostream>
#include <fstream>

using namespace std;

void decrementStackPointer(ofstream& outfile) {
  outfile << "@SP" << endl;
  outfile << "M=M-1" << endl;
}

void incrementStackPointer(ofstream& outfile) {
  outfile << "@SP" << endl;
  outfile << "M=M+1" << endl;
}

void generatePush(ofstream& outfile, string item) {
  outfile << "@" << item << endl;
  outfile << "D=M" << endl;
  outfile << "@SP" << endl;
  outfile << "A=M" << endl;
  outfile << "M=D" << endl;

  incrementStackPointer(outfile);
}

void generatePop(ofstream& outfile, string item) {
  outfile << "@R14" << endl;
  outfile << "M=M-1" << endl;
  outfile << "A=M" << endl;
  outfile << "D=M" << endl;
  outfile << "@" << item << endl;
  outfile << "M=D" << endl;
}

void generateTempPointerPush(ofstream& outfile, int pointer, int value) {
  outfile << "@" << pointer << endl;
  outfile << "D=A" << endl;
  outfile << "@" << value << endl;
  outfile << "A=D+A" << endl;
  outfile << "D=M" << endl;
  outfile << "@SP" << endl;
  outfile << "A=M" << endl;
  outfile << "M=D" << endl;
}

void generatePointerPush(ofstream& outfile, string pointer, int value) {
  outfile << "@" << pointer << endl;
  outfile << "D=M" << endl;
  outfile << "@" << value << endl;
  outfile << "A=D+A" << endl;
  outfile << "D=M" << endl;
  outfile << "@SP" << endl;
  outfile << "A=M" << endl;
  outfile << "M=D" << endl;
}

void generateTempPointerPop(ofstream& outfile, int pointer, int value) {
  outfile << "@" << pointer << endl;
  outfile << "D=A" << endl;
  outfile << "@" << value << endl;
  outfile << "D=D+A" << endl;
  outfile << "@R13" << endl;
  outfile << "M=D" << endl;

  outfile << "@SP" << endl;
  outfile << "A=M-1" << endl;
  outfile << "D=M" << endl;

  outfile << "@R13" << endl;
  outfile << "A=M" << endl;
  outfile << "M=D" << endl;
}

void generatePointerPop(ofstream& outfile, string pointer, int value) {
  outfile << "@" << pointer << endl;
  outfile << "D=M" << endl;
  outfile << "@" << value << endl;
  outfile << "D=D+A" << endl;
  outfile << "@R13" << endl;
  outfile << "M=D" << endl;

  outfile << "@SP" << endl;
  outfile << "A=M-1" << endl;
  outfile << "D=M" << endl;

  outfile << "@R13" << endl;
  outfile << "A=M" << endl;
  outfile << "M=D" << endl;
}

void generateComparisonCode(ofstream& outfile, string comparison, size_t count) {
  outfile << "@SP" << endl;
  outfile << "A=M" << endl;
  outfile << "A=A-1" << endl;
  outfile << "D=M" << endl;
  outfile << "A=A-1" << endl;
  outfile << "D=M-D" << endl;

  outfile << "@" << comparison << "$" << count << endl;
  outfile << "D;J" << comparison << endl;
  outfile << "@0" << endl;
  outfile << "D=A" << endl;
  outfile << "@end" << comparison << "$" << count << endl;
  outfile << "0;JMP" << endl;

  outfile << "(" << comparison << "$" << count << ")" << endl;
  outfile << "@0" << endl;
  outfile << "D=!A" << endl;

  outfile << "(end" << comparison << "$" << count << ")" << endl;

  outfile << "@SP" << endl;
  outfile << "A=M" << endl;
  outfile << "A=A-1" << endl;
  outfile << "A=A-1" << endl;
  outfile << "M=D" << endl;
}

void CodeGen::generateCode(vector<Command>& commands) {
  ofstream outfile{"Prog.asm"};
  size_t lts = 0;
  size_t gts = 0;
  size_t eqs = 0;
  size_t calls = 0;

  //Startup code
  outfile << "@256" << endl;
  outfile << "D=A" << endl;
  outfile << "@SP" << endl;
  outfile << "M=D" << endl;

  for(auto& command : commands) {
    if(command.getCommand() == "push") {
      if(command.getMemorySection() == "constant") {
        outfile << "@" << command.getValue() << endl;
        outfile << "D=A" << endl;
        outfile << "@SP" << endl;
        outfile << "A=M" << endl;
        outfile << "M=D" << endl;
      } else if(command.getMemorySection() == "argument") {
        generatePointerPush(outfile, "ARG", command.getValue());
      } else if(command.getMemorySection() == "local") {
        generatePointerPush(outfile, "LCL", command.getValue());
      } else if(command.getMemorySection() == "this") {
        generatePointerPush(outfile, "THIS", command.getValue());
      } else if(command.getMemorySection() == "that") {
        generatePointerPush(outfile, "THAT", command.getValue());
      } else if(command.getMemorySection() == "pointer") {
        generateTempPointerPush(outfile, 3, command.getValue());
      } else if(command.getMemorySection() == "temp") {
        generateTempPointerPush(outfile, 5, command.getValue());
      } else if(command.getMemorySection() == "static") {
        outfile << "@" << command.getFileName() << "." << command.getValue() << endl;
        outfile << "D=M" << endl;
        outfile << "@SP" << endl;
        outfile << "A=M" << endl;
        outfile << "M=D" << endl;
      }

      incrementStackPointer(outfile);
    } else if(command.getCommand() == "pop") {
      if(command.getMemorySection() == "argument") {
        generatePointerPop(outfile, "ARG", command.getValue());
      } else if(command.getMemorySection() == "local") {
        generatePointerPop(outfile, "LCL", command.getValue());
      } else if(command.getMemorySection() == "this") {
        generatePointerPop(outfile, "THIS", command.getValue());
      } else if(command.getMemorySection() == "that") {
        generatePointerPop(outfile, "THAT", command.getValue());
      } else if(command.getMemorySection() == "pointer") {
        generateTempPointerPop(outfile, 3, command.getValue());
      } else if(command.getMemorySection() == "temp") {
        generateTempPointerPop(outfile, 5, command.getValue());
      } else if(command.getMemorySection() == "static") {
        outfile << "@SP" << endl;
        outfile << "A=M-1" << endl;
        outfile << "D=M" << endl;
        outfile << "@" << command.getFileName() << "." << command.getValue() << endl;
        outfile << "M=D" << endl;
      }

      decrementStackPointer(outfile);
    } else if(command.getCommand() == "add") {
      outfile << "@SP" << endl;
      outfile << "A=M" << endl;
      outfile << "A=A-1" << endl;
      outfile << "D=M" << endl;
      outfile << "A=A-1" << endl;
      outfile << "M=D+M" << endl;

      decrementStackPointer(outfile);
    } else if(command.getCommand() == "sub") {
      outfile << "@SP" << endl;
      outfile << "A=M" << endl;
      outfile << "A=A-1" << endl;
      outfile << "D=M" << endl;
      outfile << "A=A-1" << endl;
      outfile << "M=M-D" << endl;

      decrementStackPointer(outfile);
    } else if(command.getCommand() == "neg") {
      outfile << "@SP" << endl;
      outfile << "A=M" << endl;
      outfile << "A=A-1" << endl;
      outfile << "M=-M" << endl;
    } else if(command.getCommand() == "eq") {
      generateComparisonCode(outfile, "EQ", eqs);
      eqs++;

      decrementStackPointer(outfile);
    } else if(command.getCommand() == "gt") {
      generateComparisonCode(outfile, "GT", gts);
      gts++;

      decrementStackPointer(outfile);
    } else if(command.getCommand() == "lt") {
      generateComparisonCode(outfile, "LT", lts);
      lts++;

      decrementStackPointer(outfile);
    } else if(command.getCommand() == "and") {
      outfile << "@SP" << endl;
      outfile << "A=M" << endl;
      outfile << "A=A-1" << endl;
      outfile << "D=M" << endl;
      outfile << "A=A-1" << endl;
      outfile << "M=D&M" << endl;

      decrementStackPointer(outfile);
    } else if(command.getCommand() == "or") {
      outfile << "@SP" << endl;
      outfile << "A=M" << endl;
      outfile << "A=A-1" << endl;
      outfile << "D=M" << endl;
      outfile << "A=A-1" << endl;
      outfile << "M=D|M" << endl;

      decrementStackPointer(outfile);
    } else if(command.getCommand() == "not") {
      outfile << "@SP" << endl;
      outfile << "A=M" << endl;
      outfile << "A=A-1" << endl;
      outfile << "M=!M" << endl;
    } else if(command.getCommand() == "label") {
      outfile << "(" << command.getFunctionName() << "$";
      outfile << command.getMemorySection() << ")" << endl;
    } else if(command.getCommand() == "goto") {
      outfile << "@" << command.getFunctionName() << "$" << command.getMemorySection() << endl;
      outfile << "0;JMP" << endl;
    } else if(command.getCommand() == "if-goto") {
      outfile << "@SP" << endl;
      outfile << "M=M-1" << endl;
      outfile << "A=M" << endl;
      outfile << "D=M" << endl;

      outfile << "@" << command.getFunctionName() << "$" << command.getMemorySection() << endl;
      outfile << "D;JNE" << endl;
    } else if(command.getCommand() == "call") {
      outfile << "@call$" << calls << "//Call " << command.getMemorySection() << endl;
      outfile << "D=A" << endl;
      outfile << "@SP" << endl;
      outfile << "A=M" << endl;
      outfile << "M=D" << endl;
      incrementStackPointer(outfile);

      generatePush(outfile, "LCL");
      generatePush(outfile, "ARG");
      generatePush(outfile, "THIS");
      generatePush(outfile, "THAT");

      outfile << "@SP" << endl;
      outfile << "D=M" << endl;
      outfile << "@5" << endl;
      outfile << "D=D-A" << endl;
      outfile << "@" << command.getValue() << endl;
      outfile << "D=D-A" << endl;
      outfile << "@ARG" << endl;
      outfile << "M=D" << endl;

      outfile << "@SP" << endl;
      outfile << "D=M" << endl;
      outfile << "@LCL" << endl;
      outfile << "M=D" << endl;

      outfile << "@" << command.getMemorySection() << endl;
      outfile << "0;JMP" << endl;

      outfile << "(call$" << calls << ") // End Call" << endl;

      calls++;
    } else if(command.getCommand() == "function") {
      outfile << "(" << command.getMemorySection() << ") // Function definition" << endl;
      for(int i = 0; i < command.getValue(); i++) {
        outfile << "@0" << endl;
        outfile << "D=A" << endl;
        outfile << "@SP" << endl;
        outfile << "A=M" << endl;
        outfile << "M=D" << endl;

        incrementStackPointer(outfile);
      }
    } else if(command.getCommand() == "return") {
      outfile << "@LCL // Begin return" << endl;
      outfile << "D=M" << endl;
      outfile << "@R14" << endl;
      outfile << "M=D" << endl;

      outfile << "@5" << endl;
      outfile << "A=D-A" << endl;
      outfile << "D=M" << endl;
      outfile << "@R15" << endl;
      outfile << "M=D" << endl;

      outfile << "@SP" << endl;
      outfile << "A=M-1" << endl;
      outfile << "D=M" << endl;
      outfile << "@ARG" << endl;
      outfile << "A=M" << endl;
      outfile << "M=D" << endl;

      outfile << "@ARG" << endl;
      outfile << "D=M+1" << endl;
      outfile << "@SP" << endl;
      outfile << "M=D" << endl;

      generatePop(outfile, "THAT");
      generatePop(outfile, "THIS");
      generatePop(outfile, "ARG");
      generatePop(outfile, "LCL");

      outfile << "@R15" << endl;
      outfile << "A=M" << endl;
      outfile << "0;JMP // End return" << endl;
    }
  }

  outfile.close();
}
