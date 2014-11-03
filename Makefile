
#The Compiler
Compiler = g++


#Compiler flags
Flags = -Wall -Werror -ansi -pedantic

all:
	mkdir -p bin

	$(Compiler) $(Flags) src/main.cpp -o bin/rshell
	$(Compiler) $(Flags) src/cp.cpp -o bin/cp

rshell:

	mkdir -p bin
	
	$(Compiler) $(Flags) src/main.cpp -o bin/rshell

cp:
	mkdir -p bin

	$(Compiler) $(Flags) src/cp.cpp -o bin/cp
