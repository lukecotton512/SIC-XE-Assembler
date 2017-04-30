//
// pass2.h
// Created by Luke Cotton on 4/29/17.

#ifndef _pass2_h_
#define _pass2_h_

#include <fstream>
#include "SYMTABLE.h"

// Pass 2 function.
bool pass2(std::ifstream &inputFile, std::ofstream &outputFile, size_t sizeOfProgram, int &firstAddress, SYMTABLE &symTable);

// Function to write text record to file.
void writeTextRecord(std::ofstream &outputFile, std::ostringstream &outputStream, size_t size, int locctr);

#endif