//
// pass1.cpp
// Created by Luke Cotton on 4/27/17.

#include "pass1.h"

#include <string>
#include <sstream>

#include "OPTABLE.h"

// Pass 1 function.
bool pass1(std::ifstream &inputStream, std::ofstream &outputStream, int &locctr, SYMTABLE &symtable) {
	// Initalize locctr to 0 for now.
	locctr = 0;
	
	// Check for the copy start statement.
	// Get a line from the stream.
	std::string line;
	std::getline(inputStream, line);
	
	// Whether or not we found a start statement or not.
	bool startStatement = false;
	
	// Split the stream by spaces using a string stream.
	std::istringstream lineStream (line);
	while (!lineStream.eof()) {
		// Get a part of the line.
		std::string lineInString;
		std::getline(lineStream, lineInString);
		
		// Check to see if we have the string we want.
		if (lineInString == "START") {
			// Get the next line and convert it.
			std::string lineInString;
			std::getline(lineStream, lineInString);
			
			// Try to convert it.
			try {
				locctr = stoi(lineInString);
			} catch (std::exception except) {
				// Print an error and exit.
				std::cerr << "Error: invalid token in START statement." << std::endl;
				return false;
			}
			// We found it, so set startStatement to true.
			startStatement = true;
			// Get out of here.
			break;
		}
	}
	
	// If there is not a start statement, then rewind the stream.
	if (!startStatement) {
		inputStream.seekg(0, std::ios_base::beg);
	}
	
	// Loop through the input file and get each line.
	while (!inputStream.eof()) {
		// Get a line from the stream.
		std::string line;
		std::getline(inputStream, line);
		
		// Find any tabs in the line and replace them.
		for (int i = 0; i < line.size(); i++) {
			if (line[i] == '\t') {
				// Replace the character.
				line[i] = ' ';
			}
		}
		
		// Strip any comments out of the line.
		size_t posOfDot = line.find(".");
		if (posOfDot != std::string::npos) {
			// Get rid of the rest of the line.
			line.erase(posOfDot, line.size() - posOfDot);
		}
		
		// Split the stream by spaces using a string stream.
		std::istringstream lineStream (line);
		// Also, here is our counter for doing so.
		int i = 0;
		while (!lineStream.eof()) {
			// Get a part of the string.
			std::string lineInString;
			std::getline(lineStream, lineInString, ' ');
			
			if (lineInString == "") {
				// If we are the empty string, then continue.
				continue;
			} else {
				// Check for extended mode instruction.
				bool isExtMode = false;
				std::string possiblemmenonic = lineInString;
				if (possiblemmenonic[0] == '+') {
					isExtMode = true;
					possiblemmenonic.erase(0, 1);
				}
				// Now, check to see what it is.
				OpTableEntry * entry = lookupEntryInOpTable(possiblemmenonic);
				// If it matches a memonic, then increment by the byte count.
				if (entry != nullptr) {
					// Get the length and increment the entry by the number of bytes.
					locctr += entry->format;
					// Account for ext mode instruction.
					if (isExtMode) {
						if (entry->format == 3) {
							locctr += 1;
						} else {
							// We have an invalid instruction.
							std::cerr << "Error: extended instruction is not type 3 instruction." << std::endl;
							return false;
						}
					}
				} else {
					// Check to see what else this could be.
					if (lineInString == "WORD") {
						// We have a word, so add 3 to LOCCTR.
						locctr += 3;
					} else if (lineInString == "RESW") {
						// Get the number of items in the next string, and multiply by 3.
						// Try to convert it.
						int val = 0;
						std::string valStr;
						std::getline(lineStream, valStr, ' ');
						try {
							val = stoi(valStr);
						} catch (std::exception except) {
							// Print an error and exit.
							std::cerr << "Error: invalid token in RESW statement." << std::endl;
							return false;
						}
						// Increment locctr.
						locctr += val * 3;
					} else if (lineInString == "RESB") {
						// Get the number of items in the next string, and multiply by 3.
						// Try to convert it.
						int val = 0;
						std::string valStr;
						std::getline(lineStream, valStr, ' ');
						try {
							val = stoi(valStr);
						} catch (std::exception except) {
							// Print an error and exit.
							std::cerr << "Error: invalid token in RESB statement." << std::endl;
							return false;
						}
						// Increment locctr.
						locctr += val;
					} else if (lineInString == "BYTE") {
						// If the value starts with "C'".
						size_t pos = line.find("C'");
						if (pos != std::string::npos) {
							while (line[pos] == '\'') {
								// If we are bigger than the size of the string, then return error.
								if (pos <= line.size()) {
									// Print error and return.
									std::cerr << "Error: No closing for BYTE constant" << std::endl;
									return false;
								}
								// Now, add 1 to the locctr.
								locctr += 1;
								// Add 1 to pos.
								pos++;
							}
						}
					} else if (i == 0) {
						// If this is the first line, then we probably have a symbol.
						// Get the symbol and enter it in.
						symtable.addSymbol(lineInString, locctr, 1);
					}
				}
				// Increment counter.
				i++;
			}
		}
		// End the line in the output file.
		outputStream << line << std::endl;
	}
	
	// Return success.
	return true;
}