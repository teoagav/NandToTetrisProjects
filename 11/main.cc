#include <iostream>
#include <dirent.h>
#include <deque>
#include "Tokenizer.h"
#include "Token.h"
#include "ParserCodeGen.h"

using namespace std;

int main(int argc, char** argv) {
  if(argc < 2) {
    cout << "Provide a jack file or a directory" << endl;
    return 1;
  }
  string fileName{argv[1]};
  bool jackFile = fileName.size() > 5 && fileName.substr(fileName.size() - 5, 5) == ".jack";

  Tokenizer tokenizer{};
  ParserCodeGen parserCodeGen{};

  if(jackFile) {
    deque<Token> tokens{tokenizer.tokenize(fileName, fileName)};
    parserCodeGen.parseGenerate(tokens, fileName);
  } else {
    DIR *dir;
    dirent *ent;
    dir = opendir(string("./" + fileName + "/").c_str());

    while ((ent = readdir(dir))) {
      string path = fileName + "/" + string(ent->d_name);
      string file = string(ent->d_name);

      if(file.length() > 5) {
        deque<Token> tokens{tokenizer.tokenize(path, file)};
        parserCodeGen.parseGenerate(tokens, file);
      }
    }
    closedir (dir);
  }

  return 0;
}
