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
   return label;
 }

 void Symbol::setLabel(std::string Label)
 {
  label = Label;
 }

 int Symbol::getAddress()
 {
  return Address;
 }

 void Symbol::setAddress(int address)
 {
  Address = address;
 }

 int Symbol::getLength()
 {
  return Length;
 }
 void Symbol::setLength(int length)
 {
  Length = length;
 }




