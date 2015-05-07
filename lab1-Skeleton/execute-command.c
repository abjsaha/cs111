// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"
#include <sys/wait.h> 	//for forking
#include <unistd.h> 	//for dup/dup2
#include <fcntl.h> 		//for open()
#include <error.h>

void createGraph(command_stream);
void execute_this(command_t com);

int
command_status (command_t c)
{
  return c->status;
}
int readListSize=1024;
int writeListSize=1024;
int readListIndex=0;
int writeListIndex=0;
char* readlist;
char* writelist;
typedef struct linkedListNode *linkedListNode_t;
typedef struct graphNode *graphNode_t;
linkedListNode_t linkedListHead;

struct graphNode
{
	command_t command;  		  //root of command tree
	graphNode_t* before;   //list of nodes that this node is waiting on
	pid_t pid;					  //initialized to -1, pid of process that will execute this command. if -1 then no child has been 
								  //spawn to execute this
								  //if pid >= 0, then a child has already been spawned.
	graphNode_t next;
};

typedef struct{
	graphNode_t no_dependencies; 
	graphNode_t dependencies;
} dependencyGraph;


struct linkedListNode
{
	graphNode_t gNode;
	linkedListNode_t next; 	//do we need to typedef this?
	char** RL;
	char** WL;
};

void addLinkedListNode(linkedListNode_t node)
{
	node->next = linkedListHead;
	linkedListHead->next = node;
}

void addToNoDep(graphNode_t node, dependencyGraph graph) //add a graph node to the nodependency list
{
	node->next=graph.no_dependencies;
	graph.no_dependencies->next=node;
}


void addToDep(graphNode_t node)
{
	node->next=graph.dependencies;
	graph.dependencies->next=node;
}

void processCommand(command_t cmd);

void
execute_command (command_t c, bool time_travel)
{
//execute_command takes in a command returned by read_command_stream
//command_t c is the root of a command tree
	execute_this(c);


  //error (1, 0, "command execution not yet implemented");
}

void execute_this(command_t com)
{
	//given pointer to a command
	//execute that command

	switch (com->type) 
	{
		case(SIMPLE_COMMAND):
		{
			//printf("com->input is %s\n", com->input);
			//printf("com->output is %s\n", com->output);
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
				int status;
				waitpid(p,&status,0);
				int exitStatus=WEXITSTATUS(status);
				execute_this(com->u.command[1]);
				if(exitStatus==0&&com->u.command[1]->status==0)
					com->status=0;
				else
					com->status=1;
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
				if(exitStatus==0||com->u.command[1]->status==0)
					com->status=0;
				else
					com->status=1;
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
				if(exitStatus==0&&com->u.command[1]->status==0)
					com->status=0;
				else
					com->status=1;
			}
			break;
		}
		case (PIPE_COMMAND):
		{
			//printf("com->input is %s\n", com->input);
			//printf("com->output is %s\n", com->output);
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
					execute_this(com->u.command[0]);
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
					int exit=WEXITSTATUS(status);
					com->status=exit;
				}
			}
			break;
		}
		case (SUBSHELL_COMMAND):
		{
			execute_this(com->u.subshell_command);
			if(com->u.subshell_command->status==0)
				com->status=0;
			else
				com->status=1;
			break;
		}
	}	
}

bool checkDependency(linkedListNode_t curNode, linkedListNode_t otherNode)
{
	//check WAW
	//check WAR
	//check RAW
	//if all intersections are empty
		return false;
	else
		return true;
}

DependencyGraph* createGraph(command_stream_t comStream)
{	
	//initialize graph node and linked list node:
	graphNode_t curGraphNode;
	linkedListNode_t curLinkedListNode;
	while (comStream->head)
	{
	//initialize graph node:
		curGraphNode->command = comStream->head;
		curGraphNode->pid = -1;
	//update linked list node:
		curLinkedListNode->gNode = curGraphNode;
		memset(readlist,0,strlen(readlist));
        readListIndex=0;
        readListSize=1024;
        readlist=(char*)checked_malloc(sizeof(char)*readListSize);
       	writeListIndex=0;
        writeListSize=1024;
        writelist=(char*)checked_malloc(sizeof(char)*writeListSize);
		processCommand(comStream->head->rootCommand); //add linkedlistnode parameter so we can update RL and WL in process command?
	//store linked list node in linked list:
		curLinkedListNode->RL=readlist;
		curLinkedListNode->WL=writelist;
		addLinkedListNode(curLinkedListNode);
	//Step 2: Check Dependencies:
		if (checkDependency(curLinkedListNode, otherLinkedListNode))
			//add the graph node in the otherLinkedListNode to curLinkedListNode's graphnode's before list
	//Step 3: Add to Graph
		if (curGraphNode->before) 		//before list has some content
			addToDep(curGraphNode);
		else 							//before list is empty
			addToNoDep(curGraphNode);

		comStream->head = comStream->head->next; //iterate through command stream
	}

}
void reallocReadList()
{
	readListSize*=2;
 	readlist=(char*)checked_realloc(readlist,readListSize);
}
void reallocWriteList()
{
	writeListSize*=2;
 	writelist=(char*)checked_realloc(writelist,writeListSize);
}
void processCommand(command_t cmd)
{
	if (cmd == NULL)
		return;
	if (cmd->type == SIMPLE_COMMAND)
	{
		//create RL/WL
		if(cmd->input)
		{
			//add to RL
			readlist[readListIndex++]=cmd->input;
			if(readListIndex==readListSize)
			{
				reallocReadList();
			}
		}
		if(cmd->output)
		{
			//add to WL
			writelist[writeListIndex++]=cmd->input;
			if(writeListIndex==writeListSize)
			{
				reallocWriteList();
			}
		}
		//also add words to RL ignoring options
	}
	else if (cmd->type == SUBSHELL_COMMAND)
	{
		processCommand(cmd->u.subshell_command);
	}
	else
	{
		processCommand(cmd->u.command[0]);
		processCommand(cmd->u.command[1]);
	}

}

/*
psuedo code for building the dependency graph: createGraph (command_stream)
ultimate goal is to fill in the two data structures we just created: graphNode and DependencyGraph

For each command tree K in the command stream (iterate through stream), do the following:
STEP ONE: CREATE READ LIST AND WRITE LIST
		processcommand(k->command); 	//create RL and WL for k's command tree
STEP TWO: CHECK DEPENDENCIES
			initialize graphNode to store tree K
			create a linked list node (new data structure) that contains the graphnode we just created, RL, and WL.
			insert this linked list node at the head of the linked list 
			//check if it has dependency with any of the trees procesed before it (rest of the linked list)
			//iterate through all previous command trees (rest of the linked list)
			for each previous command tree j (before k):
				check dependency between j and k by comparing read lists and write lists (WAR, RAW, WAW) which are stored in the linked list
				if there is dependency between j and k, add j to k's before list
STEP THREE: BUILD DEPENDENCY GRAPH
			if the before list is null, add this graph node into the no dependency queue
			if the before list is not null, add this graph node into the dependency queue




processCommand(command_t command){
	//generate RL and WL for each command
	//recursively iterates through command[0] and command[1]
	if cmd->type == SIMPLE_COMMAND
		store cmd->input, cmd->u.word[1] into readlist (filter out options)
		store cmd->output into writelist
	else if cmd-type == SUBSHELL_COMMAND
		store cmd->input into readlist
		store cmd->output into writelist   			//subshell commands have inputs/outputs?? ask on piazza
		processCommand(cmd->u.subshell_command)
	else 
		processCommand(cmd->u.command[0]);
		processCommand(cmd->u. command[1]);
}
*/