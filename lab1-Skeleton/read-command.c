// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include <stdio.h>
#include <error.h>
const int PRECEDENCE_SEMI_NEWLINE=1;
const int PRECEDENCE_AND_OR=2;
const int PRECEDENCE_PIPE=3;
/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */
typedef commandNode
{
  struct command* command; //root of tree
  struct commandNode* next;
};
typedef struct command_stream
{
  /* data */
  struct commandNode* head;
  struct commandNode* tail;
};
typedef struct opStackNode
{
  operator* data;
  opStackNode* next;
};
opStackNode* pop(opStackNode* current)
{
  opStackNode* tmp = cur;
  current=current->next;
  return tmp;
}
void push(opStackNode* current, opStackNode* head)
{
  current->next=head;
  head=current;
}

typedef struct comStackNode
{
  command* data;
  comStackNode* next;
};
comStackNode* pop(comStackNode* cur)
{
  opStackNode* tmp = cur;
  cur=cur->next;
  return tmp;
}
void push(comStackNode* cur, comStackNode* head)
{
  cur->next=head;
  head=cur;
}

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
  int c;
  //char** tokens = (char**) checked_malloc(CMD_SIZE * sizeof(char*));
  //tokens[0] = (char*) checked_malloc(WORD_SIZE);
  int sizeTotal=1024;
  char* entireStream=(char*)malloc(sizeof(char)*sizeTotal);
  int index=0;
  while(1) //change this to postfix transform
  {
    c=get_next_byte(get_next_byte_argument);
    if(c==EOF)
    {
      break;
    }
    entireStream[index++]=(char)c;
    if(index==sizeTotal)
    {
      sizeTotal*=2;
      entireStream=(char*)realloc(entireStream,sizeTotal);
      if(entireStream==NULL)
      {
        fprintf(stderr, "Error in re-allocationg dynamic memory");
        exit(1);
      }
    }
  }

  //make array of command trees
  //run each command tree through postfix implementation
  error (1, 0, "command reading not yet implemented");
  return 0;
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
  //error (1, 0, "command reading not yet implemented");
  //return 0;
}

opStackNode* opStackHead = null;
comStackNode* comStackHead = null;
command_stream_t;
//example: push(tmp, opStackHead);

/* PSUEDO CODE
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



void growTree(char* tmp, bool newTreeFlg, bool inputFlg, bool outputFlg){
  if (newTreeFlg){
    //add tree to stream
    //clear stacks

  else{
    operator curOp;
    command curCom;
    opStackNode* opNode;
    comStackNode* comNode;

    //determine if tmp is an operator
    //if it is an operator, set fields of curOp
    if (*tmp == "("){
      curOp.data = tmp;
      curOp.precedence = 0;
      opNode->data = curOp;
      opNode->next = null;
      push(opNode, opStackHead);
    }
    else if (*tmp == ")"){
      //while not matching (
      while (opStackHead->data.data != "(")
        //pop and combine shit
        popAndCombine();
      //create subshell command and push it to command stack
      curCom.type = SUBSHELL_COMMAND;
      curCom.subshell_command = opStackHead->data; //pop here before setting subshell_cmd?
      comNode->data = curCom;
      comNode->next = null;
      push(comNode, comStackHead);

    }
    else if (*tmp == "|"){
      curOp.data = tmp;
      curOp.precedence = PRECEDENCE_PIPE;
      opNode->data = curOp;
      opNode->next = null;
      if (opStackHead->next != null){
        //if op stack is not empty
        //while next operator on stack has greater or equal precedence than tmp
        while (opStackHead->data.precedence >= curOp.precedence && opStackHead->data.data != "(")
         //pop and combine shit
         popAndCombine();
      }
      push(opNode, opStackHead);
      }  
    }
    else if (*tmp == "||" || *tmp == "&&"){
      curOp.data = tmp;
      curOp.precedence = PRECEDENCE_AND_OR;
      opNode->data = curOp;
      opNode->next = null;
      if (opStackHead->next != null){
        //if op stack is not empty
        //while next operator on stack has greater or equal precedence than tmp
        while (opStackHead->data.precedence >= curOp.precedence && opStackHead->data.data != "(")
          //pop and combine shit
          popAndCombine();
      }
      push(opNode, opStackHead);
      }  
    }
    else if (*tmp == ";") {
      curOp.data = tmp;
      curOp.precedence = PRECEDENCE_SEMI_NEWLINE;
      opNode->data = curOp;
      opNode->next = null;
      if (opStackHead->next != null){
        //if op stack is not empty
        //while next operator on stack has greater or equal precedence than tmp
        while (opStackHead->data.precedence >= curOp.precedence && opStackHead->data.data != "(")
         //pop and combine shit
         popAndCombine();
      }
      push(opNode, opStackHead);
      }  
    }

    //tmp is a simple command
    else {
      if (inputFlg){
        //set tmp as the input of the top of command stack then push it back on command stack
        comNode = pop(comStackHead); //pointers are confusing :( i dont think i did this right
        comNode.data->input = tmp; //pointers suck
        push(comNode, comStackHead);
      }
      else if (outputFlg){
        //set tmp as output of the top of command stack then push it back on command stack
        comNode = pop(comStackHead); //pointers are confusing :( i dont think i did this right
        comNode.data->output = tmp; //pointers suck
      }
      else{
        //initialize command and push on command stack
        curCom.type = SIMPLE_COMMAND;
        //initialize command's words
        comNode->data = curCom;
        comNode->next = null;
        //push onto command stack
        push(comNode, comStackHead);
      }
    }
  }
}


popAndCombine(){
  //define command type
  if (curOp.data == "|")
    curCom.type = PIPE_COMMAND;
  if (curOp.data == "||") 
    curCom.type = OR_COMMAND;
  if (curOp.data == "&&")
    curCom.type = AND_COMMAND;
  if (curOp.data == ";")
    curCom.type = SEQUENCE_COMMAND;
  //pop two commands and combine them to be a new command
    curCom.command[0] = pop(comStackHead)->data;
    curCom.command[1] = pop(comStackHead)->data;
  //push new combined command onto command stack
    comNode->data = curCom;
    comNode->next = null;
    push(comNode, comStackHead);
}