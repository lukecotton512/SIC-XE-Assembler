 #include "Symbol.h"
 #include <string>

 Symbol::Symbol(std::string Label, int address, int length)
 {
  label = Label;
  Address = address;
  Length = length;
 }
 Symbol::~Symbol()
 {
     
 }

 std::string Symbol::getLabel()
 {
   return Symbol::label;
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




