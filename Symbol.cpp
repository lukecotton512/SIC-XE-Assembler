 #include "Symbol.h"
 #include<string>

 Symbol::Symbol(std::string Label, int address, int length)
 {
  Symbol::label = Label;
  Symbol::Address = address;
  Symbol::Length = length;
 }
 Symbol::~Symbol()
 {
     this.delete;
 }

 std::string Symbol::getLabel()
 {
   return Symbol:label;
 }

 void Symbol::setLabel(std::string Label)
 {
  Symbol::label = Label;
 }

 int Symbol::getAddress()
 {
  return Symbol::Address;
 }

 void Symbol::setAddress(int address)
 {
  Symbol::Address = address;
 }

 int Symbol::getLength()
 {
  return Symbol::Length;
 }
 void Symbol::setLength(int length)
 {
  Symbol::Length = length;
 }




