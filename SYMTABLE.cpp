 #include "SYMTABLE.h"
 #include "Symbol.h"

 SYMTABLE::SYMTABLE()
 {
  //SYMBOL TABLE STAYS EMPTY UNTIL SOURCE FILE SYMBOLS ARE ADDED TO IT.
  for (std::vector<Symbol *>::iterator it = SymbolTable.begin(); it != SymbolTable.end(); ++it) {
    // Get the symbol and delete it.
    Symbol * sym = *it;
    delete sym;
  }
 }
 void SYMTABLE::addSymbol(std::string label, int Address, int length)
 {
    Symbol *newSymbol = new Symbol(label, Address, length);                //creates new symbol and assigns the symbol value.
    SymbolTable.push_back(newSymbol);//new symbol is inserted into the vector
 }

Symbol * SYMTABLE::getSymbol(std::string label)
 {
     for(int i = 0; i < SymbolTable.size(); i++)
         {
          if(SymbolTable.at(i)->getLabel() == label)
             {
               return SymbolTable.at(i);  //Searches the SYMTABLE for a matching label and if found it returns that symbol
             }
         }
      return nullptr;   //If no matching symbol is found in the SYMTABLE then a null value is returned
 }
