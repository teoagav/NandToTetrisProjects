#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>

using namespace std;

class Command {
  string mainCommand;
  string memorySection;
  int value;
  string fileName;
  string functionName;

  public:
    Command() {}
    Command(string mainCommand, string memorySection, int value, string fileName,
      string functionName):
      mainCommand{mainCommand}, memorySection{memorySection}, value{value},
      fileName{fileName}, functionName{functionName} {}

    string& getCommand() { return mainCommand; }
    string& getMemorySection() { return memorySection; }
    int getValue() { return value; }
    string& getFileName() { return fileName; }
    string& getFunctionName() { return functionName; }


    void setCommand(string mainCommand) { this->mainCommand = mainCommand; }
    void setMemorySection(string memorySection) { this->memorySection = memorySection; }
    void setValue(int value) { this->value = value; }
    void setFileName(string fileName) { this->memorySection = fileName; }
    void setFunctionName(string fileName) { this->functionName = functionName; }

};

#endif
