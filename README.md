# Nand2Tetris Solutions

This repository contains my own solutions for the nand2tetris projects. Feel free to use for any purpose.

Nand2Tetris is a project-centered course where you build a modern computer system, from the ground up. You go all the way through, from constructing elementary logic gates to creating a fully functioning general purpose computer called HACK.

* Course page: http://nand2tetris.org/
* Part I on Coursera: https://www.coursera.org/learn/build-a-computer
* Part II on Coursera: https://www.coursera.org/learn/nand2tetris2


## What I have done so far

### Week 1

Implemented all elementary logic gates in HDL, along with more complex chips such as Multiplexers and Demultiplexers.

### Week 2

Implemented a HalfAdder, a FullAdder and the ALU.

### Week 3

Implemented sequential logic circuitry using D Flip-Flops. Built Registers, a Counter and RAM chips.

### Week 4

Developed a few programs in the HACK assembly language.

### Week 5

Implemented the CPU and the complete Memory chip with support for Screen and Keyboard memory mapping. Assembled the complete HACK computer architecture together. The HACK computer is now able to execute instructions written in the HACK assembly language.

### Week 6

Developed a 2-pass symbolic [assembler](https://github.com/Ilias95/nand2tetris/tree/master/assembler) for the HACK assembly language in C.

### Week 7

Implemented first part of a [VM translator](https://github.com/Ilias95/nand2tetris/tree/master/VM) in C. This VM translates vm code produced by a Jack compiler into HACK assembly instructions. It implements a Stack Machine and supports stack arithmetic operations such as ADD, SUB, NEG, EQ, etc. It also offers support for using different memory segments.

### Week 8

Completed the VM implementation. The VM now supports branching and implements specific calling and returning conventions for functions.

To be continued..
