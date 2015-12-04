##Program: binManipulate
Author: Seth Anderson

* binManipulate.c
* Makefile
* README

This C program reads in a file of n+1 data chunks. It then reads commands from
standard input to print data chunks in multiple formats (decimal, octal, and hex).

The program also takes commands b0, b1, b2, b3 and prints the bytes in hex.

Lastly, the program takes commands for bitwise operations and prints the output in hex.

Testing was completed for the following:
* Without a file
* With an unreadable file
* With an empty file
* With an incomplete file
* With each command with operands from stdin
* With undefined commands
* With commands and no operands

