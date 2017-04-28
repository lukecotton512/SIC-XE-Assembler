//
// pass1.h
// Created by Luke Cotton on 4/27/17.

#ifndef _pass1_h_
#define _pass1_h_

#include <iostream>
#include <fstream>
#include "SYMTABLE.h"

// Pass 1 function.
bool pass1(std::ifstream &inputStream, std::ofstream &outputStream, int &locctr, SYMTABLE &symtable);

#endif