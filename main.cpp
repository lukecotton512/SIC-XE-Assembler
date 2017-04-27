// main.cpp
// Luke Cotton
// Main file for the assembler.

#include <iostream>

#include "assemble.h"

using namespace std;
int main(int argc, char *argv[]) {
	// Check for proper number of arguments.
	if (argc > 2) {
		// Open the file as the first argument.
		std::ifstream inputFile (argv[1]);
		if (inputFile.good()) {
			// Open the output file.
			std::ofstream outputFile (argv[2]);
			if (outputFile.good()) {
				// Start pass 1.
				
				// Start pass 2.
				
				// Return 0.
				return 0;
			} else {
				// Print an error and exit.
				std::cerr "Error: error opening file." << std::endl;
				perror("Error");
				return 1;
			}
		} else {
			// Print an error and exit.
			std::cerr "Error: error opening file." << std::endl;
			perror("Error");
			return 1;
		}
	} else {
		// Print an error and exit.
		std::cerr << "Usage: " << argv[0] << " <source file> <dest file>" << std::endl;
		return 1;
	}
}