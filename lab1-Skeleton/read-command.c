
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

/* FIXME: Define the type 'struct command_stream' here.  This should
 complete the incomplete type declaration in command.h.  */
typedef commandNode
{
struct command* command; //root of tree
struct commandNode* next;
};
typedef struct command_stream
{

  struct commandNode* head;
  struct commandNode* tail;
};
typedef struct opStackNode
{
  operator* data;
  opStackNode* next;
};


commandNode* addToCommandStream(command_stream stream, commandNode newNode)
{
//if steam is empty
  if (stream.head == null)
  {
    stream.head = &newNode;
    stream.tail = &newNode;
  }
//if stream is not empty, add to end of stream
  else
  {
    tail->next = null;
    tail = &newNode;
  }
}
opStackNode* pop(opStackNode* current)
{
  opStackNode* tmp = cur;
  current=current->next;
  return tmp;
}
void push(opStackNode* current, opStackNode* head)
{
  current->next=head;
  head=current;
}


typedef struct comStackNode
{
  command* data;
  comStackNode* next;
};
comStackNode* pop(comStackNode* cur)
{
  opStackNode* tmp = cur;
  cur=cur->next;
  return tmp;
}
void push(comStackNode* cur, comStackNode* head)
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
   tempArray=(char*)malloc(sizeof(char)*INITIAL_SIZE);

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
    command nodeToAdd = pop(comStackHead)->data;
    addToCommandStream(comStream, nodeToAdd);
    //clear stacks
    comStackHead = null;
    opStackHead = null;
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
  return 0;
}
command_t
read_command_stream (command_stream_t s)
{
/* FIXME: Replace this with your implementation too.  */
//error (1, 0, "command reading not yet implemented");
  return 0;
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
          growTree(tempArray, 1,inputGlobalFlag,outputGlobalFlag);
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
          growTree(tempArray, 0,inputGlobalFlag,outputGlobalFlag);
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
          growTree(tempArray,0,inputGlobalFlag,outputGlobalFlag);
          memset(tempArray,0,strlen(tempArray));
          reallocCheck=0;
          reallocate();
          tempArray[reallocCheck++]=c;
          return c;
        }
        else//| \n \n a
        {
          twoConsNewLines=0;
          growTree(tempArray, 0,inputGlobalFlag,outputGlobalFlag);
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
        growTree(tempArray, 0,inputGlobalFlag,outputGlobalFlag);
        memset(tempArray,0,strlen(tempArray));
        reallocCheck=0; 
        reallocate();
        
        tempArray[reallocCheck++]=c;
        //realloc
        return c;
      }
      else if(prev=='<')//< a
      {
        growTree(tempArray, 0,inputGlobalFlag,outputGlobalFlag);
        memset(tempArray,0,strlen(tempArray));
        reallocCheck=0; 
        reallocate();

        tempArray[reallocCheck++]=c;
        return c;
      }
      else//| a
      {
        growTree(tempArray, 0,inputGlobalFlag,outputGlobalFlag);
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
        growTree(tempArray, 0,inputGlobalFlag,outputGlobalFlag);
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
  tempArray=(char*)realloc(tempArray,reallocSize);
}

opStackNode* opStackHead = null;
comStackNode* comStackHead = null;
command_stream_t;
command_stream comStream;

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
bool newTreeFlg2 = false;
bool inputFlg2 = false;
bool outputFlg2 = false;
operator curOp;
command curCom;
opStackNode* opNode;
comStackNode* comNode;

void growTree(char* tmp, bool newTreeFlg, bool inputFlg, bool outputFlg)
{
//reset shit
  inputGlobalFlag=0;
  outputGlobalFlag=0;
  twoConsNewLines=0;


if (newTreeFlg2){  //reached end of entire command
  //add tree to stream
  command nodeToAdd = pop(comStackHead)->data;
  addToCommandStream(comStream, nodeToAdd);
  //clear stacks
  comStackHead = null;
  opStackHead = null;
  newTreeFlg2 = false;
}

else{

  //determine if tmp is an operator
  //if it is an operator, set fields of curOp
  if (*tmp == "("){
    curOp.data = tmp;
    curOp.precedence = 0;
    opNode->data = curOp;
    opNode->next = null;
    push(opNode, opStackHead);
  }
  else if (*tmp == ")"){
    //while not matching (
    while (opStackHead->data.data != "(")
      //pop and combine shit
      popAndCombine();
    //create subshell command and push it to command stack
      curCom.type = SUBSHELL_COMMAND;
    curCom.subshell_command = opStackHead->data; //pop here before setting subshell_cmd?
    comNode->data = curCom;
    comNode->next = null;
    push(comNode, comStackHead);

  }
  else if (*tmp == "|"){
    curOp.data = tmp;
    curOp.precedence = PRECEDENCE_PIPE;
    opNode->data = curOp;
    opNode->next = null;
    if (opStackHead->next != null){
      //if op stack is not empty
      //while next operator on stack has greater or equal precedence than tmp
      while (opStackHead->data.precedence >= curOp.precedence && opStackHead->data.data != "(")
       //pop and combine shit
       popAndCombine();
     }
     push(opNode, opStackHead);
   }  

   else if (*tmp == "||" || *tmp == "&&"){
    curOp.data = tmp;
    curOp.precedence = PRECEDENCE_AND_OR;
    opNode->data = curOp;
    opNode->next = null;
    if (opStackHead->next != null){
      //if op stack is not empty
      //while next operator on stack has greater or equal precedence than tmp
      while (opStackHead->data.precedence >= curOp.precedence && opStackHead->data.data != "(")
        //pop and combine shit
        popAndCombine();
      }
      push(opNode, opStackHead);
    }  
    else if (*tmp == ";") {
      curOp.data = tmp;
      curOp.precedence = PRECEDENCE_SEMI_NEWLINE;
      opNode->data = curOp;
      opNode->next = null;
      if (opStackHead->next != null){
      //if op stack is not empty
      //while next operator on stack has greater or equal precedence than tmp
        while (opStackHead->data.precedence >= curOp.precedence && opStackHead->data.data != "(")
       //pop and combine shit
         popAndCombine();
       }
       push(opNode, opStackHead);
     }  
  //tmp is a simple command
     else {
      if (inputFlg2){
      //set tmp as the input of the top of command stack then push it back on command stack
      comNode = pop(comStackHead); //pointers are confusing :( i dont think i did this right
      comNode.data->input = tmp; //pointers suck
      push(comNode, comStackHead);
      inputFlg2 = false;
    }
    else if (outputFlg2){
      //set tmp as output of the top of command stack then push it back on command stack
      comNode = pop(comStackHead); //pointers are confusing :( i dont think i did this right
      comNode.data->output = tmp; //pointers suck
      push(comNode, comStackHead);
      outputFlg2 = false;
    }
    else{
      //initialize command and push on command stack
      curCom.type = SIMPLE_COMMAND;
      //TODO: initialize command's words
      /*char** tmpWord;
      for(int i=0;i<strlen(tmp)-1;i++)
      {
        for(int j=i+1;j<strlen(tmp);j++)
        {
          if(i==0&&tmp[j]==' ')
          {
            curCom.u.word[i]=tmp;
          }
          else if(tmp[i]==' ')
          {

          }
        }
      }
      if(strlen(tmp)==1)
        curCom.u.word[0]=tmp;*/
        comNode->data = curCom;
      comNode->next = null;
      //push onto command stack
      push(comNode, comStackHead);
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
  if (curOp.data == "|")
    curCom.type = PIPE_COMMAND;
  if (curOp.data == "||") 
    curCom.type = OR_COMMAND;
  if (curOp.data == "&&")
    curCom.type = AND_COMMAND;
  if (curOp.data == ";")
  curCom.type = SEQUENCE_COMMAND;
//pop two commands and combine them to be a new command
  curCom.command[1] = pop(comStackHead)->data;
  curCom.command[0] = pop(comStackHead)->data;
//push new combined command onto command stack
  comNode->data = curCom;
  comNode->next = null;
  push(comNode, comStackHead);
}


//DEAL WITH A<B>C<D...
