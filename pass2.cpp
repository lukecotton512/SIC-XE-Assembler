//
// pass2.cpp
// Created by Luke Cotton on 4/29/17.

#include "pass2.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include "assemble.h"

// Max length of text record.
#define LENOFRECORD 0x1D

// Pass 2 function.
bool pass2(std::ifstream &inputFile, std::ofstream &outputFile, size_t sizeOfProgram, int &firstAddress, SYMTABLE &symTable) {
	// Get the first line from the input file.
	std::string line1;
	std::getline(inputFile, line1);
	
	// Get the various variables for the program.
	std::string programName = "PROGRAM";
	int locctr = 0;
	
	// Modification records array.
	std::vector<std::string> modificationRecords;
	
	// String stream for our line.
	std::istringstream firstLineStream (line1);
	// Get the program name.
	std::getline(firstLineStream, programName, ' ');
	
	// Now, get the start parameter and discard it.
	std::string startName;
	std::getline(firstLineStream, startName, ' ');
	
	// Get the start position for the program and convert it.
	std::string startPositionStr;
	std::getline(firstLineStream, startPositionStr, ' ');
	int startPos;
	
	// Convert to string.
	try {
		startPos = stoi(startPositionStr, nullptr, 16);
	} catch (std::exception except) {
		// We have an error, so print it and exit.
		std::cerr << "Error: invalid start position." << std::endl;
		return false;
	}
	
	// Setup locctr.
	locctr = startPos;
	
	// Write the header to the file.
	// The first part of the header.
	outputFile << "H" << programName << ' ';
	// Now, the second part.
	std::ios::fmtflags f(outputFile.flags());
	outputFile << std::setfill('0') << std::setw(6) << std::hex << std::uppercase << locctr << ' ';
	outputFile << std::setfill('0') << std::setw(6) << std::hex << std::uppercase << sizeOfProgram;
	outputFile.flags(f);
	outputFile << std::endl;
	
	// Our text record.
	size_t curSizeOfRecord = 0;
	size_t curSizeOfBytesInRecord = 0;
	int locctrRecord = locctr;
	std::ostringstream textRecord;
	
	// Whether or not we are base relative, and the value in the base register.
	bool isBaseRelative = false;
	int baseRelativePosition = 0;
	
	// Now, start going line by line and parsing.
	while (!inputFile.eof()) {
		// Get a line.
		std::string line;
		std::getline(inputFile, line);
		
		// Break it up.
		std::istringstream lineStream (line);
		std::string firstPartOfLine;
		std::getline(lineStream, firstPartOfLine, ' ');
		
		// Multiply by for formatting.
		int multiplyBy = 2;
		
		// Check to see what it is.
		if (firstPartOfLine == "BASE") {
			// Set is base relative to true.
			isBaseRelative = true;
			
			// Get the next part.
			std::string nextPart;
			std::getline(lineStream, nextPart, ' ');
			
			// Lookup in the symtable.
			Symbol * symbol = symTable.getSymbol(nextPart);
			
			// If it doesn't exist, then assume it is a number and try to parse it.
			if (symbol != nullptr) {
				baseRelativePosition = symbol->getAddress();
			} else {
				// Try to convert the number.
				try {
					baseRelativePosition = stoi(nextPart, 0, 16);
				} catch (std::exception except) {
					// Print an error message and exit false.
					std::cerr << "Error: invalid base statement" << std::endl;
					return false;
				}
			}
		} else if (firstPartOfLine == "NOBASE") {
			// Set is base relative to false.
			isBaseRelative = false;
		} else if (firstPartOfLine == "RESB" || firstPartOfLine == "RESW") {
			// Size of a word.
			int sizeOfWord = 1;
			// Check to see what we are.
			if (firstPartOfLine == "RESW") {
				// If we are RESW, then we are 3.
				sizeOfWord = 3;
			} else if (firstPartOfLine == "RESB") {
				// If we are RESB, then we are 1.
				sizeOfWord = 1;
			}
			// Get the second part, which will be the number of items there.
			std::string numItemsStr;
			std::getline(lineStream, numItemsStr, ' ');
			
			// Now, convert to int.
			int numItems;
			try {
				numItems = stoi(numItemsStr);
			} catch (std::exception except) {
				// Print error and exit.
				std::cerr << "Invalid item count." << std::endl;
				return false;
			}
			
			// Increment the locctr.
			locctr += numItems * sizeOfWord;
			// Increment size of text record.
			//curSizeOfRecord += numItems * sizeOfWord;
		}
		else if (firstPartOfLine != "") {
			// Assemble the instruction.
			int byteCount;
			std::string instr;
			
			// Check the first part of the line.
			if (firstPartOfLine == "BYTE") {
				// Parse in the byte string.
				// Get the next line and check the first character.
				std::string itemStr;
				std::getline(lineStream, itemStr, ' ');
				
				// Now, check the first character.
				if (itemStr[0] == 'X') {
					// Get the characters between the parentheses and bring them in.
					byteCount = 1;
					// Get the size of the item.
					size_t itemStrSize = itemStr.size();
					if (itemStrSize != 5) {
						// Print error and return.
						std::cerr << "Error: Invalid byte constant." << std::endl;
						return false;
					}
					// Get the items and copy them in.
					std::string oneByte;
					oneByte += itemStr[2];
					oneByte += itemStr[3];
					instr += oneByte;
				} else if (itemStr[0] == 'C') {
					// Set byteCount to 0.
					byteCount = 0;
					// Get the size of the item.
					size_t itemStrSize = itemStr.size();
					if (itemStrSize < 3) {
						// Print error and return.
						std::cerr << "Error: Invalid byte constant." << std::endl;
						return false;
					}
					
					// String stream to convert byte constant to hex.
					std::ostringstream byteConstantStream;
					// Loop through and copy constant in.
					for (int i = 0; i < itemStr.size() - 3; i++) {
						byteConstantStream << std::hex << std::uppercase << int(itemStr[i+2]);
						// Now, add 1 to the byteCount.
						byteCount += 1;
					}
					// Set to instr.
					instr = byteConstantStream.str();
					// Set multiply by to 1;
					multiplyBy = 1;
				}
			} else if (firstPartOfLine == "WORD") {
				// Get the number and parse it in.
				byteCount = 3;
				
				// Get the next part.
				std::string itemStr;
				std::getline(lineStream, itemStr, ' ');
				
				// Update instr.
				instr = itemStr;
			} else if (firstPartOfLine == "END") {
				// Get out of here.
				continue;
			} else {
				// Assume it is an instruction and assemble it.
				bool needsModificationRecord = false;
				uint32_t assembledInstr = assemble(line, byteCount, locctr, isBaseRelative,  needsModificationRecord, baseRelativePosition, symTable);
				if (assembledInstr != -1) {
					// Put into string.
					std::ostringstream toStringStream;
					toStringStream << std::hex << std::uppercase << assembledInstr;
					instr = toStringStream.str();
					
					// Create a modification record if we need it.
					if (needsModificationRecord) {
						// Use a string stream to create the modification record.
						std::ostringstream modRecordStream;
						modRecordStream << 'M';
						int startOfRecord = locctr + 1;
						modRecordStream << std::setfill('0') << std::setw(6) << std::hex << startOfRecord << ' ';
						modRecordStream << "05";
						
						// Get the record and put it into the vector of records.
						std::string modRecord = modRecordStream.str();
						modificationRecords.insert(modificationRecords.end(), modRecord);
					}
				} else {
					// Get out of here.
					std::cerr << "Invalid instruction" << std::endl;
					return false;
				}
			}
			
			// If the current size plus the bytecount is smaller than the record, then print it out.
			if (curSizeOfRecord + byteCount > LENOFRECORD) {
				// Write the current record out, delete the old one, and make a new one.
				writeTextRecord(outputFile, textRecord, curSizeOfBytesInRecord, locctrRecord);
				
				// Clear the record.
				textRecord.str("");
				
				// Set new text record locctr to new position.
				locctrRecord = locctr;
				
				// Reset size of record to 0.
				curSizeOfRecord = 0;
				curSizeOfBytesInRecord = 0;
			}
			
			// Copy the instr into the text record.
			textRecord << std::setfill('0') << std::setw(byteCount * multiplyBy) << instr << ' ';
			
			// Increment locctr.
			locctr += byteCount;
			
			// Increment size.
			curSizeOfRecord += byteCount;
			curSizeOfBytesInRecord += byteCount;
		}
	}
	
	// Write final record out and delete it.
	if (curSizeOfRecord > 0) {
		writeTextRecord(outputFile, textRecord, curSizeOfBytesInRecord, locctrRecord);
	}
	
	// Write out all modification records.
	for (std::vector<std::string>::iterator it = modificationRecords.begin(); it != modificationRecords.end(); ++it) {
		// Get the record and write it out.
		std::string modificationRecord = *it;
		outputFile << modificationRecord << std::endl;
	}
	
	// Write end record.
	outputFile << 'E' << std::setfill('0') << std::setw(6) << std::hex << firstAddress;
	outputFile<< std::endl;
	outputFile.flags(f);
	
	// Return true
	return true;
}

// Function to write text record to file.
void writeTextRecord(std::ofstream &outputFile, std::ostringstream &outputStream, size_t size, int locctr) {
	// Write the beginning for the text record.
	std::ios::fmtflags f(outputFile.flags());
	outputFile << 'T';
	outputFile << std::setfill('0') << std::setw(6) << std::hex << std::uppercase << locctr << ' ';
	outputFile.flags(f);
	outputFile << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << size << ' ';
	outputFile.flags(f);
	
	// Get the string from the string stream.
	std::string outputString = outputStream.str();
	outputFile << outputString;
	
	// Reset stream back to normal.
	outputFile.flags(f);
	
	// End line.
	outputFile << std::endl;
}