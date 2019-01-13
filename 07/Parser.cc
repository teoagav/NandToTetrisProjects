#include "Parser.h"
#include <fstream>
#include <stdlib.h>
#include <iostream>

using namespace std;

vector<Command> Parser::parse(string& path, string& fileName) {
  ifstream infile{path};
  string buffer;
  vector<Command> commands{};

  size_t lineNumber = 0;
  while(getline(infile, buffer)) {
    lineNumber++;

    string cleanBuffer = "";

    //Clean up the line (comments, extra spaces, trailing spaces)
    for(size_t i = 0; i < buffer.length(); i++) {
      if(buffer[i] == '/' && buffer.length() > i+1 && buffer[i+1] == '/') {
        i = buffer.length();
      } else if(buffer[i] == ' ' || buffer[i] == '\f' || buffer[i] == '\n'
          || buffer[i] == '\r'|| buffer[i] == '\t'|| buffer[i] == '\v') {
        while(buffer.length() > i + 1 && buffer[i + 1] == ' ') {
          i++;
        }

        if(buffer.length() != i+1 || (buffer.length() > i+1 && buffer[i+1] != '/')) {
          cleanBuffer += buffer[i];
        }
      } else {
        cleanBuffer += buffer[i];
      }
    }

    //Use the "clean" command to get the different sections of the line
    if(cleanBuffer.length() > 0) {
      string command = "";
      size_t i = 0;
      while(cleanBuffer.length() > i && cleanBuffer[i] != ' ') {
        command += cleanBuffer[i];
        i++;
      }
      i++;

      string memorySection = "";
      while(cleanBuffer.length() > i && cleanBuffer[i] != ' ') {
        memorySection += cleanBuffer[i];
        i++;
      }
      i++;

      string valueString = "";
      while(cleanBuffer.length() > i) {
        valueString += cleanBuffer[i];
        i++;
      }
      int value = 0;
      if(valueString.length() > 0) {
        value = atoi(valueString.c_str());
      }

      Command newCommand{command, memorySection, value, fileName};
      commands.push_back(newCommand);
    }
  }

  return commands;
}
