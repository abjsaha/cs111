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
charStack cstack;
opStack ostack;
ostack.initStack(&ostack, 2048);
cstack.initStack(&cstack, 2048);

//initialize commands and operators as we iterate from left to right
//should we define a data struct for an operator? not sure if this is necessary
//it would have a c string and a higherPresThan function to compare presedence with other operators

//for now just define separate function higherPresThan to compare two c strings (operators)
//returns true if the first operator has a lower presedence than the second operator
bool lowerPresThan(&char cur[], &char other[]){
	if (cur == ";")
		return true;
	if (cur == "&&" || cur == "||"){

	}
}

//if current thing is a command:
//initialize the command
command com;
//set the fields of the command...
//decide what to do with the command 
if (com.type == SIMPLE_COMMAND)
	cstack.push(&cstack, com);

//if current thing is an operator
//initialize the operator
char op[];
//decide what to do with the operator
if (op == "(" || ostack.isEmpty(&ostack))
	ostack.push(&ostack, op);






