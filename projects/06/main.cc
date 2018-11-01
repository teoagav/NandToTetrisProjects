#include <iostream>
#include <vector>
#include <map>
#include "parser.h"
#include "instruction.h"
#include "codegen.h"
#include <string>

using namespace std;

int main(int argc, char** argv) {
  if(argc <= 1) {
    cout << "You must supply a file name" << endl;
    return 1;
  }

  string fileName = string{argv[1]};
  Parser parser{};
  vector<Instruction> lines = parser.parse(fileName);

  CodeGenerator codegen{};
  codegen.generate(lines);

  return 0;
}
