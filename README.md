# Project 2: Command Line Shell

See spec here: https://www.cs.usfca.edu/~mmalensek/cs521/assignments/project-2.html

# What it does:
This library implements a prototype of a shell with many of the functionalities of a simple shell.
It levarages an elastic array to hold the tokenized version of the command entered, and uses a 
circular array to hold the history of the commands the user has entered. It has the ability to use
up and down arrow keys to access previous commands and support some of the built-in commands like
`cd`, `history`, `exit`, ...

An example of the usage and results:
```
>>-[😌]-[0]-[fkaveh@root:~/P2-fika005]-> ls
a.out	 clist.h  clist.out  elist.c  elist.o  history.c  history.o    logger.h  README.md  shell.o  ui.c  ui.o    util.h 
clist.c  clist.o  crash      elist.h  fsh      history.h  libshell.so  Makefile  shell.c    tests    ui.h  util.c  util.o 
>>-[😌]-[1]-[fkaveh@root:~/P2-fika005]-> pwd
/home/fkaveh/P2-fika005
>>-[😌]-[2]-[fkaveh@root:~/P2-fika005]-> cd tests
>>-[😌]-[3]-[fkaveh@root:~/P2-fika005/tests]-> fsdf
execvp: No such file or directory
>>-[🤯]-[4]-[fkaveh@root:~/P2-fika005/tests]-> 

```
# How to build it:
run 
```
$ gcc shell.c ui.c history.c elist.c util.c clist.c -lreadline -O fsh

```
or run the make file

# What each `.c` file does:
- `shell.c`: Implements shell main and all the helper functions to process and run the commands.
- `ui.c`: Implements the ui such as the prompt, the interactive input and the access to up and down history.
- `history.c`: Implements shell history data structures and retrieval functions.
- `clist.c`: Implements the circular array to hold the history of the commands user has enetered.
- `elist.c`: It is the implementation of the elastic list. It provides a struct as the list to hold the info about the list and a pointer to its data. It
also provides some functions to manipulate the elastic list. It is mainly used to hold the tokenized version of the command.
- `util.c`: Holds some helper function to tokenize the command.
