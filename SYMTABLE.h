 #ifndef SYMTABLE_H
 #define SYMTABLE_H

 #include "Symbol.h"
 #include <vector>
 #include <string>

 class SYMTABLE {
  public:
      std::vector<Symbol *> SymbolTable; //will contain a list of all symbols

      SYMTABLE();

      void addSymbol(std::string label, int Address, int length); //allows you to insert a symbol into the vector

      Symbol * getSymbol(std::string label); //allows you to retrieve a Symbol from the symbol table
 };

#endif
