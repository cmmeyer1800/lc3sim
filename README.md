# lc3sim

This project is a LC3 simulator used to simulate binary machine code.
The LC3 language is an education language used to teach people about machine code. it consists of 16 bit instructions with 4 bit opcodes. 
Google searches of the language will show documentation of how to use the language.

The simulator is written in C++ with a python script included to test the different LC3 opcodes.

First step to running the simulator is compiling as follows:
```
$ g++ -o lc3sim lc3sim.cpp
```
The LC3 files are formatted as such:
```
0010000000000011   ; Load ascii A into R0
1001000000111111   ; Not value in R0
1111000000010101   ; Trap out
1111000000011001   ; Trap exit
1111111110111110   ; Opposite of Ascii A
```
The binary code is first and then comments are placed after

Finally to run the binary code you run the program with the filename as an argument:
```
./lc3sim filename.obj
```
The output from the simulator mimics the exact output of what the lc3 program would output
