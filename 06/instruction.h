#ifndef __INSTRUCTION__
#define __INSTRUCTION__

#include <string>

using namespace std;

class Instruction {
  public:
    enum Type {
      A_TYPE,
      C_TYPE,
      LABEL
    };
  private:
    Type type;
    string leftHandSide;
    string rightHandSide;
    string jump;

  public:
    Type getType(){ return type; }
    string getLeftHandSide(){ return leftHandSide; }
    string getRightHandSide(){ return rightHandSide; }
    string getJump(){ return jump; }

    void setType(Type type){ this->type = type; }
    void setLeftHandSide(string leftHandSide){ this->leftHandSide = leftHandSide; }
    void setRightHandSide(string rightHandSide){ this->rightHandSide = rightHandSide; }
    void setJump(string jump){ this->jump = jump; }
};

#endif
