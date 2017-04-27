 #ifndef SYMTABLE_H
 #define SYMTABEL_H

 #include "Symbol.h"
 #include <vector>
 #include <string>

 class SYMTABLE {
  public:
      vector<Symbol> SymbolTable; //will contain a list of all symbols

      SYMTABLE();

      void addSymbol(std::string label, int Address, int length); //allows you to insert a symbol into the vector

      Symbol::Symbol getSymbol(String label); //allows you to retrieve a Symbol from the symbol table
 }
