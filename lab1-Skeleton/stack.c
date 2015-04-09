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


//initialize commands and operators as we iterate from left to right
//should we define a data struct for an operator? not sure if this is necessary
//it would have a c string and a higherPresThan function to compare presedence with other operators

//for now just define separate function to compare presedence of two operators
//returns true if the other operator has a higher or equal presedence than the cur operator




