// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"

#include <error.h>

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
typedef nodeStack
{
  char* data;
  nodeStack* next;
};
nodeStack pop(nodeStack current)
{
  current=current->next;
  return current;
}
nodeStack push(nodeStack current,nodeStack head)
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
  int c = (*get_next_byte)(get_next_byte_argument);
  //char** tokens = (char**) checked_malloc(CMD_SIZE * sizeof(char*));
  //tokens[0] = (char*) checked_malloc(WORD_SIZE);
  error (1, 0, "command reading not yet implemented");
  return 0;
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
  error (1, 0, "command reading not yet implemented");
  return 0;
}
