# Makefile
# Luke Cotton
# Makefile for assembler.

all: assembler

assembler: assemble.o OPTABLE.o Symbol.o SYMTABLE.o main.o
	g++ --std=c++11 -o assembler assemble.o OPTABLE.o Symbol.o SYMTABLE.o main.o

assemble.o: assemble.cpp
	g++ --std=c++11 -o assemble.o assemble.cpp

OPTABLE.o: OPTABLE.cpp
	g++ --std=c++11 -o OPTABLE.o OPTABLE.cpp

SYMTABLE.o: SYMTABLE.cpp
	g++ --std=c++11 -o SYMTABLE.o SYMTABLE.cpp

Symbol.o: Symbol.cpp
	g++ --std=c++11 -o Symbol.o Symbol.cpp
	
main.o: main.cpp
	g++ --std=c++11 -o main.o main.cpp

clean:
	rm -rf assembler *.o
