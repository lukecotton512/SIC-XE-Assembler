 #include "SYMTABLE.h"
 #include "Symbol.h"

 SYMTABLE::SYMTABLE()
 {
  //SYMBOL TABLE STAYS EMPTY UNTIL SOURCE FILE SYMBOLS ARE ADDED TO IT
 }
 void SYMTABLE::addSymbol(std::string label, int Address, int length)
 {
    Symbol newSymbol;                //creates new symbol and assigns the symbol value.
    newSymbol.setLabel(label);
    newSymbol.setAddress(Address);
    newSymbol.setLength(length);
    SYMTABLE::SymbolTable.push_back(newSymbol);//new symbol is inserted into the vector
 }

Symbol getSymbol(std::string label)
 {
     for(int i = 0; i < SYMTABLE::SymbolTable.Size(); i++)
         {
          if(SYMTABLE::SymbolTable.at(i).getLabel() == label)
             {
               return SYMTABLE::SymbolTable.at(i);  //Searches the SYMTABLE for a matching label and if found it returns that symbol
             }
         }
      return nullptr;   //If no matching symbol is found in the SYMTABLE then a null value is returned
 }
