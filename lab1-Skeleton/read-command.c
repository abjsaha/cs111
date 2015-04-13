
// UCLA CS 111 Lab 1 command reading
#include <string.h>
#include "command.h"
#include "command-internals.h"
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#define PRECEDENCE_SEMI_NEWLINE 1
#define PRECEDENCE_AND_OR 2
#define PRECEDENCE_PIPE 3
#define INITIAL_SIZE 1024
/* FIXME: You may need to add #include directives, macro definitions,
 static function definitions, etc.  */
/*
NOTE FOR DOELLING (4/10/2015) :
As you can see a lot has changed here, almost everything is a pointer
as we want everything to exist in memory and be changed by value hence this
was essential.
ALL SYNTAX ERRORS HAVE BEEN SOLVED
implemented word handler with a O(N^2) complexity
oh and made read_command_stream
Things to do next:
> Malloc and Realloc for all pointers!!!!!!!!!!!!!!! GOD DAMN THIS IS GONNA BE ANNOYING
> Then start debugging Mdoe's part
> FINISHED WOOOHOOOO

don't over work yourself
there is always sunday
come to grace's
don't be a dumb dumd
joy out
*/
/* FIXME: Define the type 'struct command_stream' here.  This should
 complete the incomplete type declaration in command.h.  */
typedef struct commandNode *command_node_t;
typedef struct opstack *OpStackNode;
typedef struct comstack *comStackNode;
typedef struct op operator;
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
comStackNode comStackHead;
OpStackNode opStackHead;
command_stream_t comStreamT;
operator* curOp;
command_t curCom;
OpStackNode opNode;
comStackNode comNode;
bool newTreeFlg2 = false;
command_node_t addToCommandStream(command_stream_t stream, command_t newNode)
{
//if steam is empty
  command_node_t temp;
  temp->rootCommand=newNode;
  if (stream->head == NULL)
  {
    stream->head = temp;
    stream->tail = temp;
  }
//if stream is not empty, add to end of stream
  else
  {
    stream->tail->next = temp;
    stream->tail = stream->tail->next;
    stream->tail->next=NULL;
  }
}
OpStackNode popOp(OpStackNode current)
{
  OpStackNode tmp = current;
  current=current->next;
  return tmp;
}
void pushOp(OpStackNode current, OpStackNode head)
{
  current->next=head;
  head=current;
}
comStackNode popCom(comStackNode cur)
{
  comStackNode tmp = cur;
  cur=cur->next;
  return tmp;
}
void pushCom(comStackNode cur, comStackNode head)
{
  cur->next=head;
  head=cur;
}
char handleCharacter(char c, char prev, int flgFirst);
void reallocate();
void growTree(char* tmp, bool newTreeFlg, bool inputFlg, bool outputFlg);
char* tempArray;
int globalFlg=0;
int twoConsNewLines=0;
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
   tempArray=(char*)checked_malloc(sizeof(char)*INITIAL_SIZE);
   comStackHead=(comStackNode)checked_malloc(sizeof(struct comstack));
   opStackHead=(OpStackNode)checked_malloc(sizeof(struct opstack));
   comStreamT=(command_stream_t)checked_malloc(sizeof(struct command_stream));
   curOp=(operator*)checked_malloc(sizeof(struct op));
   //curOp->data=(char*)checked_realloc(INITIAL_SIZE*sizeof(char));
   curCom=(command_t)checked_malloc(sizeof(struct command));
   opNode=(OpStackNode)checked_malloc(sizeof(struct opstack));
   //opNode->data = (op*)checked_malloc(sizeof(op)); 
   //opNode->data->data=(char*)checked_malloc(INITIAL_SIZE*sizeof(char))
   comNode=(comStackNode)checked_malloc(sizeof(struct comstack));
   //comNode->data=(command*)checked_malloc(sizeof(command));
//char* entireStream=(char*)malloc(sizeof(char)*sizeTotal);
   int index=0;
   char prev=' ';
while(1) //change this to postfix transform
{
  c=get_next_byte(get_next_byte_argument);
  //printf("\n Next byte is %c",c);
  if(c==EOF)
  {

    if(globalFlg)
    {
      if(twoConsNewLines)
      {
        growTree(tempArray,1,0,0);
      }
      else
      {
        growTree(tempArray,0,0,0);
      }
    }
    else
    {
      growTree(tempArray,0,0,0);
    }
    //printf("\n reached end of file.");
    popAndCombine();
    //add tree to stream
    command_t nodeToAdd = popCom(comStackHead)->data;
    addToCommandStream(comStreamT, nodeToAdd);
    //clear stacks
    comStackHead = NULL;
    opStackHead = NULL;
    newTreeFlg2 = false;
    break;
  }
  if(index==0)
  {
    prev=handleCharacter(c,'-',index);
    //printf("\n Previous charcter is %c",prev);
    if(prev==';')
    {
     prev=' ';
     index=0;
   }
 }
 else
 {
  prev=handleCharacter(c,prev,index);
  //printf("\n Previous charcter is %c",prev);
}
index++;
  /*entireStream[index++]=(char)c;
  if(index==sizeTotal)
  {
    sizeTotal*=2;
    entireStream=(char*)realloc(entireStream,sizeTotal);
    if(entireStream==NULL)
    {
      fprintf(stderr, "Error in re-allocationg dynamic memory");
      exit(1);
    }
  }*/
  }
//growTree(tempArray,0,0,0);
//make array of command trees
//run each command tree through postfix implementation
//error (1, 0, "command reading not yet implemented");
  return comStreamT;
}
command_t
read_command_stream (command_stream_t s)
{
/* FIXME: Replace this with your implementation too.  */
//error (1, 0, "command reading not yet implemented");
  if(s->head)
  {
    command_t returnValue=s->head->rootCommand;
    command_node_t tmp=s->head;
    s->head=s->head->next;
    free(tmp);
    return returnValue;
  }
  return NULL;
}
int reallocSize=1024;
int reallocCheck=0;
int outputGlobalFlag=0;
int inputGlobalFlag=0;
int inputGlobalFlag2=0;
int outputGlobalFlag2=0;
char handleCharacter(char c, char prev, int flgFirst)
{
if(c=='>')//>
{
  outputGlobalFlag=1;
  if(prev=='\n')//\n >
  {
    twoConsNewLines=0;
    globalFlg=0;
    return c;
  }
  return c;
}
if(c=='<')//<
{
  inputGlobalFlag=1;
  if(prev=='\n')//\n <
  {
    twoConsNewLines=0;
    globalFlg=0;
    return c;
  }
  return c;
}
if(flgFirst!=0)
{
  if(c!=';'&&c!='|'&&c!='&'&&c!='('&&c!=')'&&c!='<'&&c!='>'&&c!='\n')//if current is not a special character
  {
    if(prev!=';'&&prev!='|'&&prev!='&'&&prev!='('&&prev!=')'&&prev!='<'&&prev!='>'&&prev!='\n')//if current is not a special character and previous is not a special character
    {
      tempArray[reallocCheck++]=c;
      //realloc
      if(reallocCheck==reallocSize)
      {
        reallocate();
      }
      return c;
    }
    else if(prev=='\n')//\n a
    {
      if(globalFlg)//a \n b or a \n \n b
      {
        if(twoConsNewLines)// a \n \n b
        {
          if(strcmp(tempArray," ")!=0)
          {
            if(tempArray[0]==' ')
              tempArray++;
            if(tempArray[strlen(tempArray)-1]==' ')
              tempArray[strlen(tempArray)-1]='\0';
            growTree(tempArray, 1,inputGlobalFlag,outputGlobalFlag);
          }
          memset(tempArray,0,strlen(tempArray));
          reallocCheck=0;
          globalFlg=0;
          reallocate();
          tempArray[reallocCheck++]=c;
          return c;
        }
        else//a \n b
        {
          //IFFFY
          if(strcmp(tempArray," ")!=0)
          {
            if(tempArray[0]==' ')
              tempArray++;
            if(tempArray[strlen(tempArray)-1]==' ')
              tempArray[strlen(tempArray)-1]='\0';
            growTree(tempArray, 0,inputGlobalFlag,outputGlobalFlag);
          }
          memset(tempArray,0,strlen(tempArray));
          reallocCheck=0;
          globalFlg=0;
          reallocate();
          /*tempArray[reallocCheck++]=';';
          return ';';*/
          growTree(";",0,inputGlobalFlag,outputGlobalFlag);
          tempArray[reallocCheck++]=c;
          return c;
        }
      }
      else//| \n a or | \n \n a
      {
        if (twoConsNewLines)//| \n a
        {
          twoConsNewLines=0;
          if(strcmp(tempArray," ")!=0)
          {
            if(tempArray[0]==' ')
              tempArray++;
            if(tempArray[strlen(tempArray)-1]==' ')
              tempArray[strlen(tempArray)-1]='\0';
            growTree(tempArray, 0,inputGlobalFlag,outputGlobalFlag);
          }
          memset(tempArray,0,strlen(tempArray));
          reallocCheck=0;
          reallocate();
          tempArray[reallocCheck++]=c;
          return c;
        }
        else//| \n \n a
        {
          twoConsNewLines=0;
          if(strcmp(tempArray," ")!=0)
          {
            if(tempArray[0]==' ')
              tempArray++;
            if(tempArray[strlen(tempArray)-1]==' ')
              tempArray[strlen(tempArray)-1]='\0';
            growTree(tempArray, 0,inputGlobalFlag,outputGlobalFlag);
          }
          memset(tempArray,0,strlen(tempArray));
          reallocCheck=0;
          reallocate();
          tempArray[reallocCheck++]=c;
          return c; 
        }
      }
    }
    else//if current is not special character and previous is a special character
    {
      if(prev=='>')//> a
      {
       if(strcmp(tempArray," ")!=0)
       {
        if(tempArray[0]==' ')
          tempArray++;
        if(tempArray[strlen(tempArray)-1]==' ')
          tempArray[strlen(tempArray)-1]='\0';
        growTree(tempArray, 0,inputGlobalFlag,outputGlobalFlag);
      }
      memset(tempArray,0,strlen(tempArray));
      reallocCheck=0; 
      reallocate();
      
      tempArray[reallocCheck++]=c;
        //realloc
      return c;
    }
      else if(prev=='<')//< a
      {
        if(strcmp(tempArray," ")!=0)
        {
          if(tempArray[0]==' ')
            tempArray++;
          if(tempArray[strlen(tempArray)-1]==' ')
            tempArray[strlen(tempArray)-1]='\0';
          growTree(tempArray, 0,inputGlobalFlag,outputGlobalFlag);
        }
        memset(tempArray,0,strlen(tempArray));
        reallocCheck=0; 
        reallocate();

        tempArray[reallocCheck++]=c;
        return c;
      }
      else//| a
      {
        if(strcmp(tempArray," ")!=0)
        {
          if(tempArray[0]==' ')
            tempArray++;
          if(tempArray[strlen(tempArray)-1]==' ')
            tempArray[strlen(tempArray)-1]='\0';
          growTree(tempArray, 0,inputGlobalFlag,outputGlobalFlag);
        }
        memset(tempArray,0,strlen(tempArray));
        reallocCheck=0;
        reallocate();

        tempArray[reallocCheck++]=c;
        //realloc
        return c;
      }
    }
  }
  else//if current is a special character
  {

    //if current is a special character and previous is not
    if(prev!=';'&&prev!='|'&&prev!='&'&&prev!='('&&prev!=')'&&prev!='<'&&prev!='>'&&prev!='\n')
    {
      if(c=='\n') //a \n
      {
        globalFlg=1;
        return c;
      }
      else//a |
      {
        if(strcmp(tempArray," ")!=0)
        {
          if(tempArray[0]==' ')
            tempArray++;
          if(tempArray[strlen(tempArray)-1]==' ')
            tempArray[strlen(tempArray)-1]='\0';
          growTree(tempArray, 0,inputGlobalFlag,outputGlobalFlag);
        }

        memset(tempArray,0,strlen(tempArray));
        reallocCheck=0;
        reallocate();
        

        tempArray[reallocCheck++]=c;
        //realloc
        return c;
      }
    }
    else //if current is a special character and previous is a special character
    {
      if(c=='\n'&&prev!='\n')//| \n
      {
        globalFlg=0;

        return c;
      }
      else if(c=='\n'&&prev=='\n')//\n \n
      {
        twoConsNewLines=1;
        if(globalFlg)
        {
          return c;
        }
        else
        {
          return c;
        }
      }
      else if((c=='|'&&prev=='|')||(c=='&'&&prev=='&'))//|| or &&
      {
       if(reallocCheck==reallocSize)
       {
        reallocate();
      }

      tempArray[reallocCheck++]=c;
        //realloc
      return c;
    }
      else//
      {
        if(prev=='\n')//\n |
        {
          if(twoConsNewLines)//a \n \n | or | \n \n ;
          {
            if(globalFlg)//a \n \n |
            {
             if(reallocCheck==reallocSize)
             {
               reallocate();
             }
             globalFlg=0;
             tempArray[reallocCheck++]=c;
              //realloc
             return c;
           }
            else//| \n \n ;
            {
              error (1, 0, "| \n \n ; not implemented");
              exit(0);
            }
          }
          else//a \n | or | \n ;
          {
            if (globalFlg)//a \n |
            {
              memset(tempArray,0,strlen(tempArray));
              reallocCheck=0;
              reallocate();
              tempArray[reallocCheck++]=c;
              globalFlg=0;
              //realloc
              return c;
            }
            else//| \n ;
            {
              error (1, 0, "| \n ; not yet implemented");
              exit(0);
            }
          }
        }
        else//| ;
        {
          error (1, 0, "| ; ");
          exit(0);
        }
      }
    }
  }
}
else
{
  if(c=='\n')
  {
    c=';';
    return c;
  }
  else if(c!=';'&&c!='|'&&c!='&'&&c!='('&&c!=')'&&c!='<'&&c!='>'&&c!='\n')//if current is not a special character
  {
    if(reallocCheck==reallocSize)
    {
     reallocate();
   }

   tempArray[reallocCheck++]=c;
   return c;
 }
 else
 {
  error (1, 0, "two consecutive special characters not implemented");
  exit(0);
}
}
}


/*void growTree(char* tmp, bool newTreeFlg, bool inputFlg, bool outputFlg)
{
twoConsNewLines=0;
if(newTreeFlg)
{
  //printf("\n\t\tNEW TREE");
}
if(inputGlobalFlag)
{
  //printf("\n\t\tinputFlg");
}
if(outputGlobalFlag)
{
  //printf("\n\t\toutputFlg");
}
//printf("\n\tTemp is %s",tmp);
inputGlobalFlag=0;
outputGlobalFlag=0;
}*/
void reallocate()
{
  reallocSize*=2;
  tempArray=(char*)checked_realloc(tempArray,reallocSize);
}



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

void popAndCombine();

bool inputFlg2 = false;
bool outputFlg2 = false;


void growTree(char* tmp, bool newTreeFlg, bool inputFlg, bool outputFlg)
{
//reset shit
  inputGlobalFlag=0;
  outputGlobalFlag=0;
  twoConsNewLines=0;


if (newTreeFlg2){  //reached end of entire command
  //add tree to stream
  command_t nodeToAdd = popCom(comStackHead)->data;
  addToCommandStream(comStreamT, nodeToAdd);
  //clear stacks
  comStackHead = NULL;
  opStackHead = NULL;
  newTreeFlg2 = false;
}

else{
  //curOp->data=(char*)checked_realloc(sizeof(tmp));
  
  //determine if tmp is an operator
  //if it is an operator, set fields of curOp
  if (strcmp(tmp,"(")==0){
    curOp->data = tmp;
    curOp->precedence = 0;
    //opNode->data = (op*)checked_relloc(sizeof(curOp)); 
    opNode->data = curOp;
    opNode->next = NULL;
    pushOp(opNode, opStackHead);
  }
  else if (strcmp(tmp,")")==0){
    //while not matching (
    while (strcmp(opStackHead->data->data,"(")!=0)
      //pop and combine shit
      popAndCombine();
    //create subshell command and push it to command stack
      curCom->type = SUBSHELL_COMMAND;
    curCom->u.subshell_command = opStackHead->data->data; //pop here before setting subshell_cmd?
    comNode->data = curCom;
    comNode->next = NULL;
    pushCom(comNode, comStackHead);

  }
  else if (strcmp(tmp,"|")==0){
    curOp->data = tmp;
    curOp->precedence = PRECEDENCE_PIPE;
    opNode->data = curOp;
    opNode->next = NULL;
    if (opStackHead->next != NULL){
      //if op stack is not empty
      //while next operator on stack has greater or equal precedence than tmp
      while (opStackHead->data->precedence >= curOp->precedence && strcmp(opStackHead->data->data,"(")!=0)
       //pop and combine shit
       popAndCombine();
     }
     pushOp(opNode, opStackHead);
   }  

   else if (strcmp(tmp,"||")==0 || strcmp(tmp,"&&")==0){
    curOp->data = tmp;
    curOp->precedence = PRECEDENCE_AND_OR;
    opNode->data = curOp;
    opNode->next = NULL;
    if (opStackHead->next != NULL){
      //if op stack is not empty
      //while next operator on stack has greater or equal precedence than tmp
      while (opStackHead->data->precedence >= curOp->precedence && strcmp(opStackHead->data->data,"(")!=0)
        //pop and combine shit
        popAndCombine();
      }
      pushOp(opNode, opStackHead);
    }  
    else if (strcmp(tmp,";")==0) {
      curOp->data = tmp;
      curOp->precedence = PRECEDENCE_SEMI_NEWLINE;
      opNode->data = curOp;
      opNode->next = NULL;
      if (opStackHead->next != NULL){
      //if op stack is not empty
      //while next operator on stack has greater or equal precedence than tmp
        while (opStackHead->data->precedence >= curOp->precedence && strcmp(opStackHead->data->data,"(")!=0)
       //pop and combine shit
         popAndCombine();
       }
       pushOp(opNode, opStackHead);
     }  
  //tmp is a simple command
     else {
      if (inputFlg2){
      //set tmp as the input of the top of command stack then push it back on command stack
      comNode = popCom(comStackHead); //pointers are confusing :( i dont think i did this right
      comNode->data->input = tmp; //pointers suck
      pushCom(comNode, comStackHead);
      inputFlg2 = false;
    }
    else if (outputFlg2){
      //set tmp as output of the top of command stack then push it back on command stack
      comNode = popCom(comStackHead); //pointers are confusing :( i dont think i did this right
      comNode->data->output = tmp; //pointers suck
      pushCom(comNode, comStackHead);
      outputFlg2 = false;
    }
    else{
      //initialize command and push on command stack
      curCom->type = SIMPLE_COMMAND;
      //TODO: initialize command's words
      curCom->u.word=(char**)checked_malloc(sizeof(tmp));
      int i=0, j=0, wordCounter=0;
      for(i=0;i<strlen(tmp)-1;i++)
      {
        if(tmp[i]==' '||i==0)
        {
          for(j=i+1;j<strlen(tmp);j++)
          {
            if(i==0&&tmp[j]==' ')
            {
              //memcpy(curCom->u.word[wordCounter],&tmp[i+1],j-i+1);
              curCom->u.word[wordCounter] = &tmp[i+1];
              curCom->u.word[wordCounter++][j-i+1]='\0';
              i=j;
            }
            else if(tmp[j]==' ')
            {
              //memcpy(curCom->u.word[wordCounter],&tmp[i+1],j-i+1);
              curCom->u.word[wordCounter] = &tmp[i+1];
              curCom->u.word[wordCounter++][j-i+1]='\0';
              i=j;
            }
            else if(j==strlen(tmp)-1)
            {
              //memcpy(curCom->u.word[wordCounter],&tmp[i+1],j-i+1);
              curCom->u.word[wordCounter] = &tmp[i+1];
              curCom->u.word[wordCounter++][j-i+1]='\0';
              i=j;
            }
          }
        }
      }
      if(strlen(tmp)==1)
        curCom->u.word[0]=tmp;
      comNode->data = curCom;
      comNode->next = NULL;
      //push onto command stack
      pushCom(comNode, comStackHead);
    }
  }
}


if (newTreeFlg)
{
  newTreeFlg2 = true;
  popAndCombine();
}

if (inputFlg)
  inputFlg2 = true;
if (outputFlg)
  outputFlg2 = true;

}


void popAndCombine(){
//define command type
  if (strcmp(curOp->data,"|")==0)
    curCom->type = PIPE_COMMAND;
  if (strcmp(curOp->data,"||")==0) 
    curCom->type = OR_COMMAND;
  if (strcmp(curOp->data,"&&")==0)
    curCom->type = AND_COMMAND;
  if (strcmp(curOp->data,";")==0)
  curCom->type = SEQUENCE_COMMAND;
//pop two commands and combine them to be a new command
  curCom->u.command[1] = popCom(comStackHead)->data;
  curCom->u.command[0] = popCom(comStackHead)->data;
//push new combined command onto command stack
  comNode->data = curCom;
  comNode->next = NULL;
  pushCom(comNode, comStackHead);
}


//DEAL WITH A<B>C<D...
