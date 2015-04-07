// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include <stdio.h>
#include <error.h>
const int PRECEDENCE_SEMI_NEWLINE=1;
const int PRECEDENCE_AND_OR=2;
const int PRECEDENCE_PIPE=3;
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
typedef struct opStackNode
{
  operator* data;
  opStack* next;
};
opStackNode pop(opStackNode current)
{
  opStack* tmp = cur;
  current=current->next;
  return tmp;
}
opStackNode push(opStackNode current, opStackNode head)
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
  int c;
  //char** tokens = (char**) checked_malloc(CMD_SIZE * sizeof(char*));
  //tokens[0] = (char*) checked_malloc(WORD_SIZE);
  int sizeTotal=1024;
  char* entireStream=(char*)malloc(sizeof(char)*sizeTotal);
  int index=0;
  char prev=' ';
  while(1) //change this to postfix transform
  {
    c=get_next_byte(get_next_byte_argument);
    if(c==EOF)
    {
      break;
    }
    if(index==0)
    {
      prev=handleCharacter(c,'-',index);
      if(prev==';')
        prev=' ';
      index=0;
    }
    else
    {
      prev=handleCharacter(c,prev,index);
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

  //make array of command trees
  //run each command tree through postfix implementation
  error (1, 0, "command reading not yet implemented");
  return 0;
}
void realloc()
{
  reallocSize*=2;
  tempArray=(char*)realloc(tempArray,sizeTotal);
}
command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
  //error (1, 0, "command reading not yet implemented");
  //return 0;
}
int reallocSize=1024;
int globalFlg=0;
int reallocCheck=0;
int twoConsNewLines=0;
char* tempArray=(char*)malloc(sizeof(char)*1024);
char handleCharacter(char c, char prev, int flgFirst)
{
  if(c=='>')//>
  {
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
        temp[reallocCheck++]=c;
        //realloc
        if(reallocCheck==reallocSize)
        {
          realloc();
        }
        return c;
      }
      else if(prev=='\n')//\n a
      {
        if(globalFlg)//a \n b or a \n \n b
        {
          if(twoConsNewLines)// a \n \n b
          {
            growTree(tempArray, 1,0,0);
            memset(tempArray,0,strlen(tempArray));
            reallocCheck=0;
            if(reallocCheck==reallocSize)
            {
              realloc();
            }
            tempArray[reallocCheck++]=c;
            return c;
          }
          else//a \n b
          {
            growTree(tempArray, 0,0,0);
            memset(tempArray,0,strlen(tempArray));
            reallocCheck=0;
            if(reallocCheck==reallocSize)
            { 
              realloc();
            }
            tempArray[reallocCheck++]=';';
            return c;          
          }
        }
        else//| \n a or | \n \n a
        {
          if (twoConsNewLines)//| \n a
          {
            error (1, 0, "command reading not yet implemented");
            exit(0);
          }
          else//| \n \n a
          {
            twoConsNewLines=0;
            growTree(tempArray, 0,0,0);
            memset(tempArray,0,strlen(tempArray));
            reallocCheck=0;
            if(reallocCheck==reallocSize)
            { 
              realloc();
            }
            tempArray[reallocCheck++]=c;
            return c; 
          }
        }
      }
      else//if current is not special character and previous is a special character
      {
        if(prev=='>')//> a
        {
          growTree(tempArray, 0,0,1);
          memset(tempArray,0,strlen(tempArray));
          reallocCheck=0;
          if(reallocCheck==reallocSize)
          { 
            realloc();
          }
          tempArray[reallocCheck++]=c;
          //realloc
          return c;
        }
        else if(prev=='<')//< a
        {
          growTree(tempArray, 0,1,0);
          memset(tempArray,0,strlen(tempArray));
          reallocCheck=0;
          if(reallocCheck==reallocSize)
            { 
              realloc();
            }
          tempArray[reallocCheck++]=c;
          return c;
        }
        else//| a
        {
          growTree(tempArray, 0,0,0);
          memset(tempArray,0,strlen(tempArray));
          reallocCheck=0;
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
          growTree(tempArray, 0,0,0);
          memset(tempArray,0,strlen(tempArray));
          reallocCheck=0;
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
                tempArray[reallocCheck++]=c;
                //realloc
                return c;
              }
              else//| \n \n ;
              {
                error (1, 0, "command reading not yet implemented");
                exit(0);
              }
            }
            else//a \n | or | \n ;
            {
              if (globalFlg)//a \n |
              {
                growTree(tempArray, 0,0,0);
                memset(tempArray,0,strlen(tempArray));
                reallocCheck=0;
                tempArray[reallocCheck++]=c;
                //realloc
                return c;
              }
              else//| \n ;
              {
                error (1, 0, "command reading not yet implemented");
                exit(0);
              }
            }
          }
          else//| ;
          {
            error (1, 0, "command reading not yet implemented");
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
      tempArray[reallocCheck++]=c;
      return c;
    }
    else
    {
      error (1, 0, "command reading not yet implemented");
      exit(0);
    }
  }
}