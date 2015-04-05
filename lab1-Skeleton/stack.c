stack.c

//operator and command stack implementation 1.0

#include "command.h"
#include "command-internals.h"
#include "stack.h"

//stack implementation in stack.h

/* PSUEDO CODE
infix --> postfix
implement two stacks: operator stack and command stack
parse script from left to right
- if simple command, push onto command stack
- if (, push onto operator stack
- if operator and operator stack is empty, push the operator onto the operator stack
- if operator and operator stack is not empty, pop all operators with greater/equal precedence off of the operator stack
	* for each operator, pop two commands off command stack
	  combine into new command and push it on command stack
	* stop when reach an operator with lower precedence or a (
	* push new operator onto operator stack
- if ), pop operators off until matching (
	* create subshell command by popping off top command on command stack
- if < or >, simply pop top command, set input or output field to character after < or >, push back on
*/

//initialize empty operator and command stacks
charStack opstack, comstack;
initStack(&opstack, 2048);
initStack(&comstack, 2048);


