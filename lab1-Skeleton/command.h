// UCLA CS 111 Lab 1 command interface

#include <stdbool.h>

typedef struct command *command_t;
typedef struct command_stream *command_stream_t;
typedef struct commandNode *command_node_t;
typedef struct opstack *OpStackNode;
typedef struct comstack *comStackNode;
typedef struct op operator;
typedef struct linkedListNode *linkedListNode_t;
typedef struct graphNode *graphNode_t;
typedef struct dg dependencyGraph;
struct op
{
  int precedence;
  char *data;
};
struct commandNode
{
  command_t rootCommand; //root of tree
  command_node_t next;
};
struct command_stream
{
  command_node_t head;
  command_node_t tail;
};
struct opstack
{
  operator *data;
  OpStackNode next;
};
struct comstack
{
  command_t data;
  comStackNode next;
};
struct graphNode
{
	command_t command;  		  //root of command tree
	graphNode_t* before;   //list of nodes that this node is waiting on
	pid_t pid;					  //initialized to -1, pid of process that will execute this command. if -1 then no child has been 
								  //spawn to execute this
								  //if pid >= 0, then a child has already been spawned.
	graphNode_t next;
};

struct dg
{
	graphNode_t no_dependencies; 
	graphNode_t dependencies;
};


struct linkedListNode
{
	graphNode_t gNode;
	linkedListNode_t next; 	//do we need to typedef this?
	char** RL;
	char** WL;
};
/* Create a command stream from GETBYTE and ARG.  A reader of
   the command stream will invoke GETBYTE (ARG) to get the next byte.
   GETBYTE will return the next input byte, or a negative number
   (setting errno) on failure.  */
command_stream_t make_command_stream (int (*getbyte) (void *), void *arg);

/* Read a command from STREAM; return it, or NULL on EOF.  If there is
   an error, report the error and exit instead of returning.  */
command_t read_command_stream (command_stream_t stream);

/* Print a command to stdout, for debugging.  */
void print_command (command_t);

/* Execute a command.  Use "time travel" if the flag is set.  */
void execute_command (command_t, bool);

/* Return the exit status of a command, which must have previously
   been executed.  Wait for the command, if it is not already finished.  */
int command_status (command_t);



