// main.cpp
// Luke Cotton
// Main file for the assembler.

#include <iostream>
#include <fstream>

#include "assemble.h"
#include "OPTABLE.h"
#include "SYMTABLE.h"
#include "pass1.h"

// Function prototypes.
void printIntro();

// Main function.
int main(int argc, char *argv[]) {
	// Print the intro.
	printIntro();
	// Check for proper number of arguments.
	if (argc > 2) {
		// Setup optable.
		constructOpTable();
		
		// Open the file as the first argument.
		std::ifstream inputFile (argv[1]);
		// Make sure we are open.
		if (inputFile.good()) {
			// Open the output file.
			std::ofstream outputFile (argv[2]);
			// Make sure we are open.
			if (outputFile.good()) {
				// Our symtable object.
				SYMTABLE symTable = SYMTABLE();
				// Start pass 1.
				int locctr = 0;
				if (!pass1(inputFile, outputFile, locctr, symTable)) {
					// Return error.
					return 1;
				}
				std::cout << locctr << std::endl;
				// Start pass 2.
				
				// Return 0.
				return 0;
			} else {
				// Print an error and exit.
				std::cerr << "Error: error opening file." << std::endl;
				perror("Error");
				return 1;
			}
		} else {
			// Print an error and exit.
			std::cerr << "Error: error opening file." << std::endl;
			perror("Error");
			return 1;
		}
	} else {
		// Print an error and exit.
		std::cerr << "Usage: " << argv[0] << " <source file> <dest file>" << std::endl;
		return 1;
	}
}

// Print intro message.
void printIntro() {
	// Print an intro message.
	std::cout << "SIC-XE Assembler" << std::endl;
	std::cout << "By: Luke Cotton, Matthew Anders, and Mark Miller." << std::endl;
	std::cout << std::endl;
}