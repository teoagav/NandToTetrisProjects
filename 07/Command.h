#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>

using namespace std;

class Command {
  string mainCommand;
  string memorySection;
  int value;
  string fileName;

  public:
    Command() {}
    Command(string mainCommand, string memorySection, int value, string fileName):
      mainCommand{mainCommand}, memorySection{memorySection}, value{value}, fileName{fileName} {}

    string& getCommand() { return mainCommand; }
    string& getMemorySection() { return memorySection; }
    int getValue() { return value; }
    string& getFileName() { return fileName; }

    void setCommand(string mainCommand) { this->mainCommand = mainCommand; }
    void setMemorySection(string memorySection) { this->memorySection = memorySection; }
    void setValue(int value) { this->value = value; }
    void setFileName(string fileName) { this->memorySection = fileName; }
};

#endif
