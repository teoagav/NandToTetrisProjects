#include <iostream>
#include <dirent.h>
#include <vector>
#include "Parser.h"
#include "CodeGen.h"
#include "Command.h"

using namespace std;

int main(int argc, char** argv) {
  if(argc < 2) {
    cout << "Provide a vm file name or directory";
  }
  string fileName{argv[1]};
  bool vmFile = fileName.substr(fileName.size() - 3, 3) == ".vm";

  Parser parser{};
  CodeGen codeGen{};

  if(vmFile) {
    vector<Command> commands{parser.parse(fileName, fileName)};
    codeGen.generateCode(commands);
  } else {
    DIR *dir;
    dirent *ent;
    dir = opendir(string("./" + fileName + "/").c_str());

    vector<Command> allCommands{};

    while ((ent = readdir(dir))) {
      string path = fileName + "/" + string(ent->d_name);
      string file = string(ent->d_name);
      vector<Command> commands{parser.parse(path, file)};
      allCommands.insert(allCommands.end(), commands.begin(), commands.end());
    }
    closedir (dir);

    codeGen.generateCode(allCommands);
  }

  return 0;
}
