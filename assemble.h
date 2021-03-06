// assemble.h
// Luke Cotton
// Assemble functions.

#ifndef __ASSEMBLE_H__
#define __ASSEMBLE_H__

#include <string>

#include "SYMTABLE.h"

// Assemble function.
uint32_t assemble(std::string instrStr, int &byteCount, int locctr, bool isBaseRelative, bool &needsModificationRecord, int baseRelativePosition, SYMTABLE &symTable);

// Convert register to number.
int registerNum(std::string registerVal);

#endif