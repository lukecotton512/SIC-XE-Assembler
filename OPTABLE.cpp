// OPTABLE.cpp
// Luke Cotton
// Functions for handling the OPTABLE.

#include "OPTABLE.h"
#include "OPTABLEStr.h"

#include <fstream>
#include <sstream>

#define SPACE ' '

void constructOpTable() {
	// Break the string up into each line.
	std::string optableStr = OPTABLEStr;
	std::istringstream sStream (optableStr);
	std::string inputStr;
	while (!sStream.eof()) {
		// Split into each instruction.
		std::getline(sStream, inputStr);
		
		// Split the instructions into 3 parts.
		std::istringstream inputsStream (inputStr);
		std::string mmenonicstr;
		std::string formatstr;
		std::string opcodestr;
		std::getline(inputsStream, mmenonicstr, SPACE);
		std::getline(inputsStream, formatstr, SPACE);
		std::getline(inputsStream, opcodestr, SPACE);
		
		// Convert the format and the opcode to a string.
		int format = std::stoi(formatstr);
		int opcode = std::stoi(opcodestr, nullptr, 16);
		
		// Put opcode into OPTABLE.
		OpTableEntry entry;
		entry.instrStr = mmenonicstr;
		entry.format = format;
		entry.opcode = opcode;
		OPTABLE[opcode] = entry;
	}
}

// Lookup entry by mmenonic.
OpTableEntry * lookupEntryInOpTable(std::string mmenonic) {
	// Loop through the whole optable and find the instruction.
	for (int i = 0; i < 256; i++) {
		// Get an entry and check it, and return if we have it.
		OpTableEntry *entry = &OPTABLE[i];
		if (entry->instrStr == mmenonic) {
			return entry;
		}
	}
	// If we've gotten here, then we don't have a corresponding instruction, so return NULL.
	return nullptr;
}