// OPTABLE.h
// Luke Cotton.
// Defines the OPTABLE.

#include <string>

#ifndef __OPTABLE_H__
#define __OPTABLE_H__

// Our structure for an entry in the OPTABLE.
typedef struct OpTableEntry {
	std::string instrStr;
	int format;
	int opcode;
} OpTableEntry;

// Our OPTABLE.
static OpTableEntry * OPTABLE;

// Function to construct optable. 
// We need to run this before doing anything in the assembler.
void constructOpTable();

// Looks up a entry by the mmenonic.
OpTableEntry * lookupEntryInOpTable(std::string mmenonic);

#endif