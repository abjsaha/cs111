// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"
#include <sys/wait.h> 	//for forking
#include <unistd.h> 	//for dup
#include <fcnt1.h> 		//for open()
#include <error.h>

command_t execute_this(command_t com);

int
command_status (command_t c)
{
  return c->status;
}


void
execute_command (command_t c, bool time_travel)
{
//execute_command takes in a command returned by read_command_stream
//command_t c is the root of a command tree

	//parse/iterate through command tree LCR
	//send the command to be executed next to execute_this
	execute_this(...);


  //error (1, 0, "command execution not yet implemented");
}

command_t execute_this(command_t com)
{
	//given pointer to a command
	//execute that command

	if (com->type == SIMPE_COMMAND)
	if (com->type == SEQUENCE_COMMAND)
	if (com->type == OR_COMMAND)
	if (com->type == AND_COMMAND)
	if (com->type == PIPE_COMMAND)
	if (com->type == SUBSHELL_COMMAND)
}