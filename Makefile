
#The Compiler
Compiler = g++


#Compiler flags
Flags = -Wall -Werror -ansi -pedantic

all:
	mkdir -p bin

	$(Compiler) $(Flags) src/main.cpp -o bin/rshell

rshell:

	mkdir -p bin
	
	$(Compiler) $(Flags) src/main.cpp -o bin/rshell	
