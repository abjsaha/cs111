// UCLA CS 111 Lab 1 command execution
#include <string.h>
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
int readListSize=1024;
int writeListSize=1024;
int readListIndex=0;
int writeListIndex=0;
int readListSizeTracker=0;
int writeListSizeTracker=0;
char** readlist;
char** writelist;
typedef struct linkedListNode *linkedListNode_t;
typedef struct graphNode *graphNode_t;
typedef struct dg dependencyGraph;
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

void addLinkedListNode(linkedListNode_t node)
{
	node->next = linkedListHead;
	linkedListHead->next = node;
}

void addToNoDep(graphNode_t node, dependencyGraph* graph) //add a graph node to the nodependency list
{
	node->next=graph->no_dependencies;
	graph->no_dependencies->next=node;
}


void addToDep(graphNode_t node, dependencyGraph* graph)
{
	node->next=graph->dependencies;
	graph->dependencies->next=node;
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
int wordLength(char** words)
{
	int size=0;
	while(words[size]!=NULL)size++;
	return size;
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
	int curWLlen = wordLength(curNode->WL);
	int curRLlen = wordLength(curNode->RL);
	int otherWLlen = wordLength(otherNode->WL);
	int otherRLlen = wordLength(otherNode->RL);
	//check WAW
	int i = 0, j = 0;
	for (i = 0; i < curWLlen; i++)
	{
		for (j = 0; j < otherWLlen; j++)
		{
			if (strcmp(curNode->WL[i], otherNode->WL[j]))
				return true;
		}
	}
	//check WAR
	for (i = 0; i < curWLlen; i++)
	{
		for (j = 0; j < otherRLlen; j++)
		{
			if (strcmp(curNode->WL[i], otherNode->RL[j]))
				return true;
		}
	}
	//check RAW
	for (i = 0; i < curRLlen; i++)
	{
		for (j = 0; j < otherWLlen; j++)
		{
			if (strcmp(curNode->RL[i], otherNode->WL[j]))
				return true;
		}
	}
	
	return false;
}

dependencyGraph* createGraph(command_stream_t comStream)
{	
	//initialize graph node and linked list node:
	graphNode_t curGraphNode;
	linkedListNode_t curLinkedListNode;
	linkedListNode_t temp;
	while (comStream->head)
	{
	//initialize graph node:
		curGraphNode->command = comStream->head;
		curGraphNode->pid = -1;
	//update linked list node:
		curLinkedListNode->gNode = curGraphNode;
		int i=0;
		for(i=0;i<readListIndex;i++)
		{
			memset(readlist[i],0,strlen(readlist[i]));
		}
		for(i=0;i<writeListIndex;i++)
		{
			memset(writelist[i],0,strlen(writelist[i]));
		}
		memset(readlist,0,readListSizeTracker);
		memset(writelist,0,writeListSizeTracker);
        readListIndex=0;
        readListSizeTracker=0;
        readListSize=1024;
        readlist=(char**)checked_malloc(sizeof(char*)*readListSize);
       	writeListIndex=0;
       	writeListSizeTracker=0;
        writeListSize=1024;
        writelist=(char**)checked_malloc(sizeof(char*)*writeListSize);
		processCommand(comStream->head->rootCommand); //add linkedlistnode parameter so we can update RL and WL in process command?
	//store linked list node in linked list:
		curLinkedListNode->RL=readlist;
		curLinkedListNode->WL=writelist;
		addLinkedListNode(curLinkedListNode);
	//Step 2: Check Dependencies:
		temp=curLinkedListNode->next;
		while(temp)
		{
			if (checkDependency(curLinkedListNode, temp))
			{

			}
			//add the graph node in the otherLinkedListNode to curLinkedListNode's graphnode's before list
			temp=temp->next;
		}
	//Step 3: Add to Graph
		dependencyGraph actual;
		actual->no_dependencies=NULL;
		actual->dependencies=NULL;
		dependencyGraph* sent=&actual;
		if (curGraphNode->before) 		//before list has some content
			addToDep(curGraphNode, sent);
		else 							//before list is empty
			addToNoDep(curGraphNode, sent);

		comStream->head = comStream->head->next; //iterate through command stream
	}
	return sent;
}
void reallocReadList()
{
	readListSize*=2;
 	readlist=(char**)checked_realloc(readlist,readListSize);
}
void reallocWriteList()
{
	writeListSize*=2;
 	writelist=(char**)checked_realloc(writelist,writeListSize);
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
			readlist[readListSizeTracker++]=cmd->input;
			readListIndex+=strlen(cmd->input);
			if(readListIndex==readListSize)
			{
				reallocReadList();
			}
		}
		if(cmd->output)
		{
			//add to WL
			writelist[writeListSizeTracker++]=cmd->output;
			writeListIndex+=strlen(cmd->output);
			if(writeListIndex==writeListSize)
			{
				reallocWriteList();
			}
		}
		//also add words to RL ignoring options
		int i=0;
		int length=wordLength(cmd->u.word);
		for(i=0;i<length;i++)
		{
			if(cmd->u.word[i][0]=='-')
				continue;
			else
			{
				readlist[readListSizeTracker++]=cmd->u.word[i];
				readListIndex+=strlen(cmd->u.word[i]);
				if(readListIndex==readListSize)
				{
					reallocReadList();
				}
			}
		}
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
void executeNoDependencies(graphNode_t nodep)
{
	while(nodep)
	{
		pid_t pid=fork();
		if(pid==0)
		{
			execute_command(nodep->command,true);
			exit(0);
		}
		else
		{
			nodep->pid=pid;
		}
		nodep=nodep->next;
	}
}
int executeDependencies(graphNode_t dep)
{
	while(dep)
	{
		int status;
		int i=0;
		while(dep->before[i])
		{
			waitpid(dep->before[i]->pid,&status,0);
			i++;
		}
		pid_t pid=fork();
		if(pid==0)
		{
			execute_command(dep->command,true);
			exit(0);
		}
		dep=dep->next;
	}
}
int executeGraph(dependencyGraph* mainGraph)
{
	executeNoDependencies(mainGraph->no_dependencies);
	int status=executeDependencies(mainGraph->dependencies);
	return status;
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