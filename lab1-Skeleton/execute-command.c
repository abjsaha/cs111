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

	switch (com->type) 
	{
		case(SIMPLE_COMMAND):
		{
			printf("com->input is %s\n", com->input);
			printf("com->output is %s\n", com->output);
			int p=fork();
			if(p==0)
			{
				//input
				if (com->input)
				{
					int fd1 = open(com->input, O_RDONLY);
					if (fd1 < 0)
						error(1, 0, "could not open input file");
					int fd2 = 0;
					int fd_dup = dup2(fd1, fd2);
					if (fd_dup != 0)
						error(1, 0, "failed to redirect command input");
					//execvp(com->u.word[0],com->u.word);
				}
				//output
				if (com->output)
				{
					int fd1 = open(com->output, O_CREAT | O_TRUNC | O_WRONLY, 0644);
					if (fd1 < 0)
						error(1, 0, "could not open output file");
					int fd2 = 1;
					dup2(fd1, fd2);
					//execvp(com->u.word[0],com->u.word);
				}
				//no output or input
				if (!com->output && !com->input )
					printf("no input or output");
				execvp(com->u.word[0],com->u.word);
			}
			else
			{
				int status;
				waitpid(p,&status,0);
				int exitStatus=WEXITSTATUS(status);
				com->status=exitStatus;
			}
			break;
		}
		case (SEQUENCE_COMMAND):
		{
			//execute
			int p=fork();
			if(p==0)
			{
				execute_this(com->u.command[0]);
			}
			else
			{
				execute_this(com->u.command[1]);
			}
			break;
		}
		case (OR_COMMAND)://deal with if information is successful
		{
			//execute
			int p=fork();
			if(p==0)
			{
				execute_this(com->u.command[0]);
			}
			else
			{
				int status;
				waitpid(p,&status,0);
				int exitStatus=WEXITSTATUS(status);
				com->u.command[1]->status=exitStatus;
				if(exitStatus==1)
				{
					execute_this(com->u.command[1]);
				}
			}
			break;
		}
		case (AND_COMMAND)://deal with if information is not successful
		{
			//execute
			int p=fork();
			if(p==0)
			{
				execute_this(com->u.command[0]);
			}
			else
			{
				int status;
				waitpid(p,&status,0);
				int exitStatus=WEXITSTATUS(status);
				if(exitStatus==0)
				{
					execute_this(com->u.command[1]);
				}
			}
			break;
		}
		case (PIPE_COMMAND):
		{
			printf("com->input is %s\n", com->input);
			printf("com->output is %s\n", com->output);
			int fd[2];
			if(pipe(fd)==-1)
			{
				error(1, 0, "error in creating pipe");
			}
			int firstPid=fork();
			if(firstPid==-1)
				error(1, 0, "error in creating fork");
			if(firstPid==0)
			{
				//execute command on right
				close(fd[1]);
				dup2(fd[0],0);
				execute_this(com->u.command[1]);
			}
			else
			{
				int secondPid=fork();
				if(secondPid==-1)
					error(1, 0, "error in creating fork");
				if(secondPid==0)
				{
					//execute command on left
					close(fd[0]);
					dup2(fd[1],1);
					execute_this(com->u.command[1]);
				}
				else
				{
					close(fd[0]);
					close(fd[1]);
					int status;
					int returnedPid=waitpid(-1,&status,0);
					if(returnedPid==secondPid)
					{
						waitpid(firstPid,&status,0);
					}
					if(returnedPid==firstPid)
					{
						waitpid(secondPid,&status,0);
					}
				}
			}
			break;
		}
		case (SUBSHELL_COMMAND):
		{
			execute_this(com->u.subshell_command);
			break;
		}
	}	
}