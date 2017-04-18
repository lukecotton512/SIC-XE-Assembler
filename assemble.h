// assemble.h
// Luke Cotton
// Assemble functions.

#ifndef __ASSEMBLE_H__
#define __ASSEMBLE_H__

#include <string>

// Assemble function.
char * assemble(std::string instrStr, int &byteCount, int locctr);

// Convert register to number.
int registerNum(std::string registerVal);

#endif