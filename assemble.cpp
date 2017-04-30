// assemble.cpp
// Luke Cotton
// The assemble function of our assembler.

#include "assemble.h"

#include "OPTABLE.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <unistd.h>

// Assemble function.
uint32_t assemble(std::string instrStr, int &byteCount, int locctr, bool isBaseRelative, bool &needsModificationRecord, int baseRelativePosition, SYMTABLE &symTable) {
	// Split into multiple parts.
	std::istringstream sStream(instrStr);
	std::string mmemonic;
	std::getline(sStream, mmemonic, ' ');
	
	// Set needs modification record to false first.
	needsModificationRecord = false;
	
	// Check first character and check for plus sign.
	// If so, we have an extended mode instruction.
	bool isExtMode = false;
	if (mmemonic[0] == '+') {
		isExtMode = true;
		mmemonic.erase(0, 1);
	}
	
	// Look for the function in our entry table.
	OpTableEntry * entry = lookupEntryInOpTable(mmemonic);
	
	// If the entry is null, we have an error, so get out of here.
	if (entry == nullptr) {
		std::cerr << "Error: invalid instruction mmemonic." << std::endl;
		return -1;
	} else {
		// Buffer for instruction.
		uint32_t assembledInstr = 0;
		// We have something, so handle the instruction.
		if (entry->format == 1) {
			// Type 1 instruction.
			// Set to opcode.
			assembledInstr += entry->opcode;
			
		} else if (entry->format == 2) {
			// Type 2 instruction.
			// Byte count is 2.
			byteCount = 2;
			
			// If we have nothing else in the string stream, then get out of here.
			if (sStream.eof()) {
				std::cerr << "Error: Invalid value!" << std::endl;
				exit(EXIT_FAILURE);
				return -1;
			}
			
			// Grab register 1.
			std::string register1;
			std::getline(sStream, register1, ',');
			// Get the next register.
			std::string register2;
			std::getline(sStream, register2);
			// If there is an excessive space, then get rid of it.
			if (register2[0] == ' ') {
				register2.erase(0,1);
			}
			
			// Convert to integer values.
			int reg1Val = registerNum(register1);
			int reg2Val = registerNum(register2);
			
			// Check for errors.
			if (reg1Val == -1) {
				std::cerr << "Error: Invalid register!" << std::endl;
				return -1;
			}
			// If we don't have the second register, then assume we don't need a second register.
			if (reg2Val == -1) {
				reg2Val = 0;
			}
			
			// Put opcode in assembledInstr and shift by 8 bytes.
			assembledInstr += entry->opcode;
			assembledInstr = assembledInstr << 8;
			
			// Put register value in.
			assembledInstr += reg1Val << 4;
			assembledInstr += reg2Val;
			
		} else if (entry->format == 3) {
			// Check for extended mode.
			if (isExtMode) {
				// Extended mode instruction.
				byteCount = 4;
				// Plug opcode in and push over by 24 bytes.
				assembledInstr += entry->opcode;
				assembledInstr = assembledInstr << 24;
				
				// Make sure we have a label.
				if (sStream.eof() && mmemonic != "RSUB") {
					std::cerr << "Error: no memory address for type 3/4 instruction." << std::endl;
					return -1;
				}
				
				// Get our label.
				std::string memoryAddressStr;
				std::getline(sStream, memoryAddressStr);
				
				// Our variables for immediate and relative addressing.
				bool isRelative = true;
				bool isImmediate = true;
				
				// Check the first character.
				char firstChar = memoryAddressStr[0];
				if (firstChar == '#') {
					// Mark that this is immediate addressing and modify the appropriate variables.
					isRelative = false;
					// Erase the character from the string.
					memoryAddressStr.erase(0,1);
				} else if (firstChar == '@') {
					// Mark that this is relative addressing and modify the appropriate variables.
					isImmediate = false;
					// Erase the character from the string.
					memoryAddressStr.erase(0,1);
				}
				
				// Now, set the control bits to their proper values.
				// And then push them in.
				uint32_t bitmask = 0x00000001;
				// Relative addressing.
				if (isRelative) {
					// Add 2 to the bitmask.
					bitmask += 0x20;
				}
				// Is immediate.
				if (isImmediate) {
					// Add 1 to the bitmask.
					bitmask += 0x10;
				}
				bitmask = bitmask << 20;
				assembledInstr += bitmask;
				
				// Now, lookup the label.
				Symbol * symTabEntry = symTable.getSymbol(memoryAddressStr);
				// Get the address.
				uint32_t memoryAddress;
				if (symTabEntry != nullptr) {
					// Get the memory address for the symbol.
					memoryAddress = symTabEntry->getAddress();
					// Also, set the modification record boolean value to true.
					needsModificationRecord = true;
				} else {
					// If null, then try to convert to memory address.
					try {
						int base = 16;
						if (isImmediate) {
							base = 10;
						}
						memoryAddress = stoi(memoryAddressStr, 0, base);
					} catch (std::exception except) {
						// If we are not RSUB.
						if (mmemonic != "RSUB") {
							// We have an error, so get out of here.
							std::cerr << "Error: invalid token in label field." << std::endl;
							return -1;
						} else {
							memoryAddress = 0x00000000;
						}
					}
				}
				
				// If the address is bigger than the allowed amount, then error out as well.
				if (memoryAddress > 0xFFFFF) {
					std::cerr << "Error: Invalid memory address." << std::endl;
					return -1;
				} else {
					// Put the memory address in the instruction.
					assembledInstr += memoryAddress;
				}
				
			} else {
				// Normal type 3 instruction.
				byteCount = 3;
				
				// Is immediate and is
				// Plug opcode in and push over by 16 bytes.
				assembledInstr += entry->opcode;
				assembledInstr = assembledInstr << 16;
				
				// Make sure we have a label.
				if (sStream.eof() && mmemonic != "RSUB") {
					std::cerr << "Error: no memory address for type 3/4 instruction." << std::endl;
					return -1;
				}
				
				// Get our label.
				std::string memoryAddressStr;
				std::getline(sStream, memoryAddressStr);
				
				// Make sure we are not index addressed.
				bool isIndexAddressed = false;
				size_t pos = memoryAddressStr.find(",X");
				if (pos != std::string::npos) {
					// Set index addressing boolean, and erase last of characters.
					isIndexAddressed = true;
					memoryAddressStr.erase(pos);
				} 
				
				// Try with space.
				pos = memoryAddressStr.find(", X");
				if (pos != std::string::npos) {
					// Set index addressing boolean, and erase last of characters.
					isIndexAddressed = true;
					memoryAddressStr.erase(pos);
				} 
				
				// Our variables for immediate and relative addressing.
				bool isRelative = true;
				bool isImmediate = true;
				
				// Check the first character.
				char firstChar = memoryAddressStr[0];
				if (firstChar == '#') {
					// Mark that this is immediate addressing and modify the appropriate variables.
					isRelative = false;
					// Erase the character from the string.
					memoryAddressStr.erase(0,1);
				} else if (firstChar == '@') {
					// Mark that this is relative addressing and modify the appropriate variables.
					isImmediate = false;
					// Erase the character from the string.
					memoryAddressStr.erase(0,1);
				}
				
				// Lookup the symbol, and get the memory address.
				// Now, lookup the label.
				Symbol * symTabEntry = symTable.getSymbol(memoryAddressStr);
				// Get the address.
				uint32_t memoryAddress;
				if (symTabEntry != nullptr) {
					memoryAddress = symTabEntry->getAddress();
				} else {
					// If null, then try to convert to memory address.
					try {
						int base = 16;
						if (isImmediate) {
							base = 10;
						}
						memoryAddress = stoi(memoryAddressStr, 0, base);
					} catch (std::exception except) {
						// If we are not RSUB.
						if (mmemonic != "RSUB") {
							// We have an error, so get out of here.
							std::cerr << "Error: invalid token in label field." << std::endl;
							return -1;
						} else {
							memoryAddress = 0x00000000;
						}
					}
				}
				
				// Perform relative addressing using the locctr.
				int32_t relativeAddress;
				if (isRelative) {
					// Check for base relativity.
					if (isBaseRelative) {
						// We are base relative.
						relativeAddress = memoryAddress - baseRelativePosition;
					} else {
						// We are not base relative.
						relativeAddress = memoryAddress - locctr - byteCount;
					}
				} else {
					relativeAddress = memoryAddress;
				}
				
				// Zero out higher bits.
				relativeAddress = relativeAddress & 0x00000FFF;
				
				// If the address is bigger than the allowed amount, then error out as well.
				if (relativeAddress > 0xFFF) {
					std::cerr << "Error: Invalid memory address." << std::endl;
					return -1;
				} else {
					// Put the memory address in the instruction.
					assembledInstr += relativeAddress;
				}
				
				// Now, set the control bits to their proper values.
				
				uint32_t bitmask = 0x00000000;
				// Relative addressing.
				if (isRelative) {
					// Add 2 to the bitmask.
					bitmask += 0x20;
				}
				// Is immediate.
				if (isImmediate) {
					// Add 1 to the bitmask.
					bitmask += 0x10;
				}
				// Is index addressed.
				if (isIndexAddressed) {
					// Add the index bit to the bitmask.
					bitmask += 0x8;
				}
				// Make it PC relative or base relative.
				if (isRelative) {
					if (isBaseRelative) {
						bitmask += 0x4;
					} else {
						bitmask += 0x2;	
					}
				}
				// And then push them in.
				bitmask = bitmask << 12;
				assembledInstr += bitmask;
			}
		}
		// Return the instruction.
		return assembledInstr;
	}
	
}

// Convert register to number.
int registerNum(std::string registerVal) {
	if (registerVal == "A") {
		// Accumulator.
		return 0;
	} else if (registerVal == "X") {
		// Index.
		return 1;
	} else if (registerVal == "L") {
		// Linkage.
		return 2;
	} else if (registerVal == "B") {
		// Base.
		return 3;
	} else if (registerVal == "S") {
		// General working register.
		return 4;
	} else if (registerVal == "T") {
		// General working register.
		return 5;
	} else if (registerVal == "F") {
		// Floating point register.
		return 6;
	} else if (registerVal == "PC") {
		// Program counter.
		return 8;
	} else if (registerVal == "SW") {
		// Status word.
		return 9;
	} else {
		// Invalid.
		return -1;
	}
}