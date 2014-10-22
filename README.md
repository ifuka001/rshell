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

main.cpp
```
```
./tests:

exec.script
```

How to run file once you have it cloned into your machine.
---
**RECOMMENDED USING LINUX**
In order to run the program, follow these steps:


1. clone it to your machine `git clone http://github.com/ifuka001/rshell.git`
2. `cd rshell` which will move you to the directory `rshell`
3. `git checkout hw0` which will move you to the current state where the program is.
4. `make` to compile the program
5. `bin/rshell` to run the program itself.

Rshell should be now opened instead of your bash and it will be able to execute most of the commands except few. For example the cd command will not work yet.

Features of this program
---

* `rshell` can do most of the commands BASH can excluding few for the moment.
* `rshell` supports the usage of arguments, such as, `;` `&&` `||`
* Just like the actual BASH itself, the program will find your login name and host name and will display it along with the `$` prompt.
* To terminate the program, you must enter `exit` in the command line.
* You may use `#` to make comments in your program.

Known BUGS and ISSUES
---
* `rshell` cannot use the command 'cd' at this time because it is not implemented.
* `rshell` does not allow you to combine all the arguments. For example, `ls -a; pwd || echo "hi" && mkdir temp` `ls && pwd || echo "goodbye"`
* In `rshell` the commands `ls` and/or with flags will work for now, but in the future we will add in the code to make it work better.
* `rshell` does not support the usage of `TAB` key to find other commands with a similar letters typed in. It will read it as a user input and just print out an error message.
* It will also not allow you to echo a line of string into a file like BASH does. Instead it will just print out what you wrote after the echo command (excluding the comments made after a `#`)







