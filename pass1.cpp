//
// pass1.cpp
// Created by Luke Cotton on 4/27/17.

#include "pass1.h"

#include <string>
#include <sstream>

#include "OPTABLE.h"

// Pass 1 function.
bool pass1(std::ifstream &inputStream, std::ofstream &outputStream, int &locctr, int &startExecution, SYMTABLE &symtable) {
	// Initalize locctr to 0 for now.
	locctr = 0;
	
	// Where we want to start execution.
	startExecution = 0;
	
	// Check for the copy start statement.
	// Get a line from the stream.
	std::string line;
	std::getline(inputStream, line);
	
	// Whether or not we found a start statement or not.
	bool startStatement = false;
	
	// Find any tabs in the line and replace them.
	for (int i = 0; i < line.size(); i++) {
		if (line[i] == '\t') {
			// Replace the character.
			line[i] = ' ';
		}
	}
	
	// Find any '\r' characters in the file and replace them with spaces.
	for (int i = 0; i < line.size(); i++) {
		if (line[i] == '\r') {
			// Replace the character.
			line[i] = ' ';
		}
	}
	
	// Strip any comments out of the line.
	size_t posOfDot = line.find(".");
	if (posOfDot != std::string::npos) {
		// Get rid of the rest of the line.
		line.erase(posOfDot, line.size() - posOfDot);
		// If the last character is a space, then get rid of it.
		if (line[line.size()-1] == ' ') {
			line.erase(line.size() - 1, 1);
		}
	}
		
	// Split the stream by spaces using a string stream.
	std::istringstream lineStream (line);
	while (!lineStream.eof()) {
		// Get a part of the line.
		std::string lineInString;
		std::getline(lineStream, lineInString, ' ');
		
		// Check to see if we have the string we want.
		if (lineInString == "START") {
			// Get the next line and convert it.
			std::string lineInString;
			std::getline(lineStream, lineInString);
			
			// Try to convert it.
			try {
				locctr = stoi(lineInString, nullptr, 16);
			} catch (std::exception except) {
				// Print an error and exit.
				std::cerr << "Error: invalid token in START statement." << std::endl;
				return false;
			}
			// We found it, so set startStatement to true.
			startStatement = true;
			// Set start position to locctr.
			startExecution = locctr;
			// End the line in the output file.
			outputStream << line << std::endl;
			// Get out of here.
			break;
		}
	}
	
	// If there is not a start statement, then rewind the stream.
	if (!startStatement) {
		inputStream.seekg(0, std::ios_base::beg);
		
		// Also, proceed to write a generic start statement into the program.
		outputStream << "PROGRAM START 0" << std::endl;
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
		
		// Find any '\r' characters in the file and replace them with spaces.
		for (int i = 0; i < line.size(); i++) {
			if (line[i] == '\r') {
				// Replace the character.
				line[i] = ' ';
			}
		}
		
		// Strip any comments out of the line.
		size_t posOfDot = line.find(".");
		if (posOfDot != std::string::npos) {
			// Get rid of the rest of the line.
			line.erase(posOfDot, line.size() - posOfDot);
			// If the last character is a space, then get rid of it.
			if (line[line.size()-1] == ' ') {
				line.erase(line.size() - 1, 1);
			}
		}
		
		// Make a copy of the line for later.
		std::string lineCopy = line;
		
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
						// Get the number of items in the next string, and add them in.
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
						size_t oneBytePos = line.find("X'");
						if (pos != std::string::npos) {
							// Increment pos by 1.
							pos += 2;
							// Look for how much we have with the string constant.
							while (line[pos] != '\'') {
								// If we are bigger than the size of the string, then return error.
								if (pos > line.size()) {
									// Print error and return.
									std::cerr << "Error: No closing for BYTE constant" << std::endl;
									return false;
								}
								// Now, add 1 to the locctr.
								locctr += 1;
								// Add 1 to pos.
								pos++;
							}
						} else if (oneBytePos != std::string::npos) {
							// Add 1 to the location counter for one byte.
							locctr += 1;
						}
					} else if (lineInString == "BASE" || lineInString == "NOBASE") {
						// We are a base statement, so ignore.
					} else if (lineInString == "END") {
						// We are an end statement, so get the address of the first executable place.
						
						// Check for an existing label.
						int val = 0;
						std::string valStr;
						std::getline(lineStream, valStr, ' ');
						// See if the entry is in the symtable.
						Symbol * symbol = symtable.getSymbol(valStr);
						if (symbol != nullptr) {
							// Assign to address from symtable.
							val = symbol->getAddress();
						} else {
							// Try to convert it.
							try {
								val = stoi(valStr, 0, 16);
							} catch (std::exception except) {
								// Print an error and exit.
								std::cerr << "Error: invalid token in END statement." << std::endl;
								return false;
							}
						}
						// Set start location to value.
						startExecution = val;
					} else if (i == 0) {
						// If this is the first line, then we probably have a symbol.
						// Get the symbol and enter it in.
						symtable.addSymbol(lineInString, locctr, 1);
						
						// Also, remove it from the copy of the line.
						size_t labelPos = lineCopy.find(lineInString);
						lineCopy.erase(labelPos, lineInString.size());
					}
				}
				// Increment counter.
				i++;
			}
		}
		// Get rid of any spaces at the start of the input line.
		while (lineCopy[0] == ' ') {
			// Delete the character.
			lineCopy.erase(0,1);
		}
		// Get rid of any spaces at the end too.
		while (lineCopy[lineCopy.size() - 1] == ' ') {
			// Delete the character.
			lineCopy.erase(lineCopy.size() - 1,1);
		}
		// End the line in the output file.
		outputStream << lineCopy << std::endl;
	}
	
	// Return success.
	return true;
}