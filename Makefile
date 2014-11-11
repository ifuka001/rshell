
#The Compiler
Compiler = g++


#Compiler flags
Flags = -Wall -Werror -ansi -pedantic

all:
	mkdir -p bin

	$(Compiler) $(Flags) src/main.cpp -o bin/rshell
	$(Compiler) $(Flags) src/cp.cpp -o bin/cp
	$(Compiler) $(Flags) src/ls.cpp -o bin/ls
	
rshell:

	mkdir -p bin
	
	$(Compiler) $(Flags) src/main.cpp -o bin/rshell

ls:
	mkdir -p bin

	$(Compiler) $(Flags) src/ls.cpp -o bin/ls

cp:
	mkdir -p bin

	$(Compiler) $(Flags) src/cp.cpp -o bin/cp
