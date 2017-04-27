// assemble.h
// Luke Cotton
// Assemble functions.

#ifndef __ASSEMBLE_H__
#define __ASSEMBLE_H__

#include <string>

#include "SYMTABLE.h"

// Assemble function.
char * assemble(std::string instrStr, int &byteCount, int locctr, SYMTABLE &symTable);

// Convert register to number.
int registerNum(std::string registerVal);

#endif