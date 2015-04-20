// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"
#include <sys/wait.h> 	//for forking
#include <unistd.h> 	//for dup/dup2
#include <fcntl.h> 		//for open()
#include <error.h>

void execute_this(command_t com);

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
	execute_this(c);


  //error (1, 0, "command execution not yet implemented");
}

void execute_this(command_t com)//TODO: deal with not returning to main process
{
	//given pointer to a command
	//execute that command

	if (com->type == SIMPLE_COMMAND)
	{
		//output
		if (com->output)
		{
			printf("com->output is %s\n", com->output);
			int fd1 = open(com->output, O_CREAT | O_TRUNC | O_WRONLY, 0644);
			if (fd1 < 0)
				error(1, 0, "could not open output file");
			int fd2 = 1;
			dup2(fd1, fd2);
			execvp(com->u.word[0],com->u.word);
		}
		//input
		if (com->input)
		{
			printf("com->input is %s\n", com->output);
			int fd1 = open(com->input, O_CREAT | O_TRUNC | O_WRONLY, 0644);
			if (fd1 < 0)
				error(1, 0, "could not open input file");
			int fd2 = 0;
			int fd_dup = dup2(fd1, fd2);
			if (fd_dup != 0)
				error(1, 0, "failed to redirect command input");
			execvp(com->u.word[0],com->u.word);
		}
		//no output or input
		if (!com->output && !com->input )
			execvp(com->u.word[0],com->u.word);
	}
	else if (com->type == SEQUENCE_COMMAND)
	{
		//execute
		int p=fork();
		if(p==0)
		{
			if(com->u.command[0]->type==SIMPLE_COMMAND)
				execvp(com->u.command[0]->u.word[0],com->u.command[0]->u.word);
			else
				execute_this(com->u.command[0]);
		}
		else
		{
			if(com->u.command[1]->type==SIMPLE_COMMAND)
				execvp(com->u.command[1]->u.word[0],com->u.command[1]->u.word);
			else
				execute_this(com->u.command[1]);
		}
	}
	else if (com->type == OR_COMMAND)//deal with if information is successful
	{
		//execute
		int p=fork();
		if(p==0)
		{
			if(com->u.command[0]->type==SIMPLE_COMMAND)
				execvp(com->u.command[0]->u.word[0],com->u.command[0]->u.word);
			else
				execute_this(com->u.command[0]);
		}
		else
		{
			int status;
			waitpid(p,&status,0);
			int exitStatus=WEXITSTATUS(status);
			if(exitStatus==1)
			{
				if(com->u.command[1]->type==SIMPLE_COMMAND)
				{
					execvp(com->u.command[1]->u.word[0],com->u.command[1]->u.word);
				}
				else
				{
					execute_this(com->u.command[1]);
				}
			}
		}
	}
	else if (com->type == AND_COMMAND)//deal with if information is not successful
	{
		//execute
		int p=fork();
		if(p==0)
		{
			if(com->u.command[0]->type==SIMPLE_COMMAND)
				execvp(com->u.command[0]->u.word[0],com->u.command[0]->u.word);
			else
				execute_this(com->u.command[0]);
		}
		else
		{
			int status;
			waitpid(p,&status,0);
			int exitStatus=WEXITSTATUS(status);
			if(exitStatus==0)
			{
				if(com->u.command[1]->type==SIMPLE_COMMAND)
				{
					execvp(com->u.command[1]->u.word[0],com->u.command[1]->u.word);
				}
				else
				{
					execute_this(com->u.command[1]);
				}
			}
		}
	}
	else if (com->type == PIPE_COMMAND)
	{

	}
	else if (com->type == SUBSHELL_COMMAND)
	{
		execute_this(com->u.subshell_command);
	}
}