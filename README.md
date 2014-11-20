RSHELL
======

Licensing information: READ LICENSE
---

Project source can be downloaded from https://github.com/ifuka001/rshell.git
---

Author & Contributor list
---

Itsuki Fukada

Any other known bug and fixes can be sent to ifuka001@ucr.edu

File List
----
```
.:

Makefile

LICENSE

README.md

./src

./tests
```
```
/src:
ls.cpp
Timer.h
cp.cpp
main.cpp
```
```
./tests:

ls.script
exec.script
pipe.script
redirect.script
```

How to run file once you have it cloned into your machine.
---
**RECOMMENDED USING LINUX**
In order to run the program, follow these steps:


1. `git clone http://github.com/ifuka001/rshell.git`
2. `cd rshell` 
4. `make`
5. `bin/rshell` for rshell. `bin/ls` for ls `bin/cp` for cp

Rshell should be now opened instead of your bash and it will be able to execute most of the commands except few. For example the cd command will not work yet.

updated(11/11/2014):
added ls feature and cp feature. these will work like the actualy ls and cp. (ls only has -R -l and -a flags compatible.)

updated(11/19/2014):
added i/o redirection and pipe in rshell.

Features of this program
---

* `rshell` can do most of the commands BASH can excluding few for the moment.
* `rshell` supports the usage of arguments, such as, `;` `&&` `||`
* Just like the actual BASH itself, the program will find your login name and host name and will display it along with the `$` prompt.
* To terminate the program, you must enter `exit` in the command line.
* You may use `#` to make comments in your program.

11/11/2014:
* Added ls and cp. ls has 3 flags -l -a and -R and they can be used in many ways. ( -l, -Ra, -lRa, etc)
* Added cp. if an argument was passed through after the Source and the Destination (it can be anything), it will output 3 different methods and how long each one took to output it to the file.
* Note that the cp cannot take flags in the beginning or between the source file and the destination file.

11/19/2014:
* Added input/output redirection and piping in rshell program. It is able to do simple input "<" and output ">" redirection ( and append ">>")


Known BUGS and ISSUES
---
* `rshell` cannot use the command 'cd' at this time because it is not implemented.
* `rshell` does not allow you to combine all the arguments. For example, `ls -a; pwd || echo "hi" && mkdir temp` `ls && pwd || echo "goodbye"`
* In `rshell` the commands `ls` and/or with flags will work for now, but in the future we will add in the code to make it work better.
* `rshell` does not support the usage of `TAB` key to find other commands with a similar letters typed in. It will read it as a user input and just print out an error message.
* It will also not allow you to echo a line of string into a file like BASH does. Instead it will just print out what you wrote after the echo command (excluding the comments made after a `#`)
* `ls` if the file name length is longer than 20characters, it might cause issue when outputting the file.
* `ls` recommended bash size is 80width to show the output neat.
* `cp` can only take flag parameter at the end of destination or it will not run properly.
* For example: `cp` only runs by inputting `bin/cp "sourcefile" destinationfile"` or `bin/cp "sourcefile" destinationfile" "flag"`
* `rshell` piping does not support multiple i/o redirection within the pipe itself (for example, cat< Makefile| cat <Makefile >> new1)
* `rshell` also does not allow multiple input and output redirections. it does not also allow output and append to be on same line of command




