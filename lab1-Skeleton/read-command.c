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
typedef struct opStack
{
  operator* data;
  opStack* next;
};
opStack pop(opStack current)
{
  current=current->next;
  return current;
}
opStack push(opStack current, opStack head)
{
  current->next=head;
  head=current;
  return head;
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
