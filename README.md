# 8086x-lite-assembler
A simulated processor with small memory and registers using a very extremely lite 8086x assembly code

Part 6:	Input 10 numbers into an array which are bigger than 0 or smaller than 100, add them and output the sum

Part 7:	Input 10 numbers into an array which are bigger than 0 or smaller than 100, add them and return sum by reference

Part 8:	Input 10 numbers into an array and sort them using bubble sort

Part 9:	Create a linked list, user inputs numbers until he enters 0 or exceeds the memory limit

tictactoeasm.asm : Tic-Tac-Toe game in assembly. Game is played in the last memory positions.

## Commands:
1. mov reg constant
2. mov reg reg
3. mov reg address
4. mov address reg
5. mov reg [bx]
6. mov [bx] reg
7. mov [bx + constant] reg
8. mov reg [bx + constant]
9. add reg []
10. add reg constant
11. add reg reg
12. sub reg reg - subtract
13. cmp reg reg
14. cmp reg address
15. cmp reg [bx]
16. cmp reg constant
17. jump commands
18. fun address numOfParameters- function
19. ret address - return
20. put - output value of ax
21. get - get value from user and put in ax
22. dump - printMemoryDump();
