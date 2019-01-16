#ifndef __SYMBOL_TABLE_ITEM_H__
#define __SYMBOL_TABLE_ITEM_H__

#include <string>

using namespace std;

class SymbolTableItem {
  string type;
  string kind;
  size_t number;

  public:
    SymbolTableItem() {}
    SymbolTableItem(string type, string kind, size_t number):
      type{type}, kind{kind}, number{number} {}

    string& getType() { return type; }
    string& getKind() { return kind; }
    size_t getNumber() { return number; }

    void setType(string type) { this->type = type; }
    void setKind(string kind) { this->kind = kind; }
    void setNumber(size_t number) { this->number = number; }
};

#endif
