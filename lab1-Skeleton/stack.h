stack.h

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "command-internals.h"
#include "command.h"

//for now implemented as a char stack
//should be able to hold strings?
//do we need to implement two separate stacks, one for operators and one for commands?
typedef struct{
	char* contents;
	int top;
	int maxSize;
	void initStack(charStack *stackPtr, int maxsize);
	void destroyStack(charStack *stackPtr);
	void push(charStack *stackPtr, char element);
	char pop(charStack *stackPtr); //does this need to return char*?
	bool isEmpty(charStack *stackPtr);
	bool isFull(charStack *stackPtr);
} charStack;

void charStack::initStack(charStack *stackPtr, int maxsize) {
	char *newContents;
	newContents = (char *)malloc(sizeof(char)*maxsize);
	if (newContents == NULL) {
		fprintf(stderr, "failed to initialize stack.\n");
		exit(1);
	}
	stackPtr->contents = newContents;
	stackPtr->maxSize = maxsize;
	stackPtr->top = -1
}

void charStack::destroyStack(charStack *stackPtr){
	free(stackPtr->contents);
	stackPtr->contents = NULL;
	stackPtr->maxSize = 0;
	stackPtr->top = -1;
}

bool charStack::isEmpty(charStack *stackPtr){
	return (stackPtr->top < 0)
}

bool charStack::isFull(charStack *stackPtr){
	return (stackPtr->top >= stackPtr->maxSize - 1);
}

void charStack::push(charStack *stackPtr, char element) {
	if (isFull(stackPtr)) {
		//stack is full
		//reallocate new stack with larger capacity
	}
	stackPtr->contents[++stackPtr->top] = element;
}

char charStack::pop(charStack *stackPtr){
	if (isEmpty(stackPtr))
		return NULL;
	return stackPtr->contents[stackPtr->top--];
}




//same stack implementation for command data type:

typedef struct{
	command* contents;
	int top;
	int maxSize;
	void initStack(comStack *stackPtr, int maxsize);
	void destroyStack(comStack *stackPtr);
	void push(comStack *stackPtr, command element);
	command pop(comStack *stackPtr);
	bool isEmpty(comStack *stackPtr);
	bool isFull(comStack *stackPtr);
} comStack;

void charStack::initStack(comStack *stackPtr, int maxsize) {
	command *newContents;
	newContents = (command *)malloc(sizeof(command)*maxsize);
	if (newContents == NULL) {
		fprintf(stderr, "failed to initialize stack.\n");
		exit(1);
	}
	stackPtr->contents = newContents;
	stackPtr->maxSize = maxsize;
	stackPtr->top = -1
}

void charStack::destroyStack(comStack *stackPtr){
	free(stackPtr->contents);
	stackPtr->contents = NULL;
	stackPtr->maxSize = 0;
	stackPtr->top = -1;
}

bool charStack::isEmpty(comStack *stackPtr){
	return (stackPtr->top < 0)
}

bool charStack::isFull(comStack *stackPtr){
	return (stackPtr->top >= stackPtr->maxSize - 1);
}

void charStack::push(comStack *stackPtr, command element) {
	if (isFull(stackPtr)) {
		//stack is full
		//reallocate new stack with larger capacity
	}
	stackPtr->contents[++stackPtr->top] = element;
}

command charStack::pop(comStack *stackPtr){
	if (isEmpty(stackPtr))
		return NULL;
	return stackPtr->contents[stackPtr->top--];
}