# Towers of Hanoi

This is a simple C program to solve the famous [Towers of Hanoi](http://en.wikipedia.org/wiki/Tower_of_Hanoi) problem. It makes use of ncurses library for graphics.

It takes as input the number of discs (n) for which to solve the puzzle and the speed of running the execution.

The program features a progress bar and current completion of execution in percentage.

## How to run

First, make sure that you have the ncurses library installed.

Then, run the following commands to run the program:

<pre>
gcc hanoi.c -lncurses
./a.out
</pre>

![Demo](https://raw.githubusercontent.com/Koderok/towers-of-hanoi/master/assets/hanoi-demo.gif)
