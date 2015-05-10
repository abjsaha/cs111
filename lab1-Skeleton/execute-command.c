// UCLA CS 111 Lab 1 command execution
#include <string.h>
#include "command.h"
#include "command-internals.h"
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
linkedListNode_t linkedListHead;
dependencyGraph* graph;

void addLinkedListNode(linkedListNode_t node)
{
	/*if(!linkedListHead)
		linkedListHead=node;
	else
	{*/
		node->next = linkedListHead;
		linkedListHead = node;
	//}
}

void addToNoDep(graphNode_t node) //add a graph node to the nodependency list
{
	/*if(!graph->no_dependencies)
		graph->no_dependencies=node;
	else
	{*/
		node->next=graph->no_dependencies;
		graph->no_dependencies=node;
	//}
}


void addToDep(graphNode_t node)
{
	/*if(!graph->dependencies)
		graph->dependencies=node;
	else
	{*/
		node->next=graph->dependencies;
		graph->dependencies=node;
	//}
}

void processCommand(command_t cmd, linkedListNode_t curLinkedListNode);

void
execute_command (command_t c, bool time_travel)
{
//execute_command takes in a command returned by read_command_stream
//command_t c is the root of a command tree
	int p=fork();
	if(p==0)
		execute_this(c);
	else
	{
		int status;
		waitpid(p,&status,0);
	}


  //error (1, 0, "command execution not yet implemented");
}
int wordLength(char** words)
{
	int size=0;
	if(words)
		while(words[size]&&words[size][0])size++;
	return size;
}
void execute_this(command_t com)
{
	//given pointer to a command
	//execute that command
	//input
	if (com->input)
	{
		int fd1 = open(com->input, O_RDONLY, 0644);
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
	//if (!com->output && !com->input )
	//	printf("no input or output");
	switch (com->type) 
	{
		case(SIMPLE_COMMAND):
		{
			//printf("com->input is %s\n", com->input);
			//printf("com->output is %s\n", com->output);
			
			int p=fork();
			if(p==0)
			{
				
				execvp(com->u.word[0],com->u.word);
				exit(127);
			}
			else
			{
				int status;
				waitpid(p,&status,0);
				int exitStatus=WEXITSTATUS(status);
				com->status=exitStatus;
				exit(exitStatus);
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
				//close(fd[0]);
				//exit(com->u.command[1]->status);
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
					//close(fd[1]);
					//exit(com->u.command[1]->status);
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
					int exitStatus=WEXITSTATUS(status);
					com->status=exitStatus;
					exit(1);
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
	linkedListHead=(linkedListNode_t)checked_malloc(sizeof(struct linkedListNode));
	//initialize graph node and linked list node:
	int indexBefore=0;
	int sizeBefore=1024;
	//dependencyGraph actual=(dependencyGraph)checked_malloc(sizeof(struct dg));
	//actual.no_dependencies=NULL;
	//actual.dependencies=NULL;
	graph=(dependencyGraph*)checked_malloc(sizeof(dependencyGraph));
    graphNode_t curGraphNode=(graphNode_t)checked_malloc(sizeof(struct graphNode));
	linkedListNode_t curLinkedListNode=(linkedListNode_t)checked_malloc(sizeof(struct linkedListNode));
	linkedListNode_t temp;//=(linkedListNode_t)checked_malloc(sizeof(struct linkedListNode));
	while (comStream->head)
	{
	//initialize graph node:
		curGraphNode=NULL;
		curLinkedListNode=NULL;
		curLinkedListNode=(linkedListNode_t)checked_malloc(sizeof(struct linkedListNode));
		curGraphNode=(graphNode_t)checked_malloc(sizeof(struct graphNode));
		curGraphNode->command = comStream->head->rootCommand;
		curGraphNode->pid = -1;
	//update linked list node:
		curLinkedListNode->gNode = curGraphNode;
		int i=0;
		/*for(i=0;i<readListSizeTracker;i++)
		{
			memset(readlist[i],0,strlen(readlist[i]));
		}
		for(i=0;i<writeListSizeTracker;i++)
		{
			memset(writelist[i],0,strlen(writelist[i]));
		}
		memset(readlist,0,readListSizeTracker);
		memset(writelist,0,writeListSizeTracker);*/
        readListIndex=0;
        readListSizeTracker=0;
        readListSize=1024;
        readlist=(char**)checked_malloc(sizeof(char*)*readListSize);
       	writeListIndex=0;
       	writeListSizeTracker=0;
        writeListSize=1024;
        curLinkedListNode->RL=(char**)checked_malloc(sizeof(char*)*readListSize);
		curLinkedListNode->WL=(char**)checked_malloc(sizeof(char*)*writeListSize);
        writelist=(char**)checked_malloc(sizeof(char*)*writeListSize);
		processCommand(comStream->head->rootCommand, curLinkedListNode); //add linkedlistnode parameter so we can update RL and WL in process command?
	//store linked list node in linked list:
		/*curLinkedListNode->RL=(char**)checked_malloc(sizeof(char*)*readListSize);
		curLinkedListNode->WL=(char**)checked_malloc(sizeof(char*)*writeListSize);
		for(i=0;i<readListSizeTracker;i++)
		{
			curLinkedListNode->RL[i]=(readlist[i]);
		}
		for(i=0;i<writeListSizeTracker;i++)
		{
			curLinkedListNode->WL[i]=writelist[i];
		}*/
		curLinkedListNode->next=NULL;
		addLinkedListNode(curLinkedListNode);
	//Step 2: Check Dependencies:
		temp=linkedListHead->next;
		//curLinkedListNode->gNode=(graphNode_t)checked_malloc(sizeof(graphNode)*)
		linkedListHead->gNode->before=(graphNode_t*)checked_malloc(sizeof(graphNode_t)*sizeBefore);
		while(temp)
		{
			if (checkDependency(linkedListHead, temp))
			{
				linkedListHead->gNode->before[indexBefore++]=temp->gNode;
				if(indexBefore==sizeBefore)
				{
					sizeBefore*=2;
					linkedListHead->gNode->before=(graphNode_t*)checked_realloc(linkedListHead->gNode->before,sizeBefore);
				}
			}
			//add the graph node in the otherLinkedListNode to curLinkedListNode's graphnode's before list
			temp=temp->next;
		}
	//Step 3: Add to Graph
		curGraphNode->next=NULL;
		if (linkedListHead->gNode->before[0]) 		//before list has some content
			addToDep(curGraphNode);
		else 							//before list is empty
			addToNoDep(curGraphNode);

		comStream->head = comStream->head->next; //iterate through command stream
	}
	return graph;
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
void processCommand(command_t cmd, linkedListNode_t curLinkedListNode)
{
	if (cmd == NULL)
		return;
	if (cmd->type == SIMPLE_COMMAND)
	{
		//create RL/WL
		if(cmd->input)
		{
			//add to RL
			curLinkedListNode->RL[readListSizeTracker++]=cmd->input;
			readListIndex+=strlen(cmd->input);
			if(readListIndex==readListSize)
			{
				reallocReadList();
			}
		}
		if(cmd->output)
		{
			//add to WL
			curLinkedListNode->WL[writeListSizeTracker++]=cmd->output;
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
				curLinkedListNode->RL[readListSizeTracker++]=cmd->u.word[i];
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
		processCommand(cmd->u.subshell_command, curLinkedListNode);
	}
	else
	{
		processCommand(cmd->u.command[0], curLinkedListNode);
		processCommand(cmd->u.command[1], curLinkedListNode);
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
		outer: ;
		int j=0;
		while(dep->before[j])
		{
			if(dep->before[j]->pid==-1)
			{
				goto outer;
			}
			j++;
		}
		
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
		else
		{
			dep->pid=pid;
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