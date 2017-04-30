// main.cpp
// Luke Cotton
// Main file for the assembler.

#include <iostream>
#include <fstream>

#include "assemble.h"
#include "OPTABLE.h"
#include "SYMTABLE.h"
#include "pass1.h"
#include "pass2.h"

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
				// Intermediate file path.
				const char interPath[] = ".jfhekqds-SIC-XE-Assembler-Intermediate";
				// Open an intermediate file in the current directory.
				std::ofstream intermediateFile (interPath);
				// Make sure it is open and exit if not.
				if (!intermediateFile.good()) {
					// We have an error, so exit.
					std::cerr << "Error: error creating intermediate file." << std::endl;
					perror("Error");
					return 1;
				}
				
				// Our symtable object.
				SYMTABLE symTable = SYMTABLE();
				
				// Start pass 1.
				int locctr = 0;
				int startPosition = 0;
				std::cout << "Running Pass 1." << std::endl;
				if (!pass1(inputFile, intermediateFile, locctr, startPosition, symTable)) {
					// Remove temp file and return error.
					remove(interPath);
					return 1;
				}
				
				// Open intermediate file as input.
				std::ifstream inputInterFile(interPath);
				// Make sure it is open and exit if not.
				if (!inputInterFile.good()) {
					// We have an error, so exit.
					std::cerr << "Error: error creating intermediate file." << std::endl;
					perror("Error");
					return 1;
				}
				
				// Start pass 2.
				std::cout << "Running Pass 2." << std::endl;
				if (!pass2(inputInterFile, outputFile, locctr, startPosition, symTable)) {
					// Remove temp file and return error.
					remove(interPath);
					return 1;
				}
				
				// Remove temp file, print done, and return 0.
				remove(interPath);
				std::cout << "Done" << std::endl;
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