stack.h

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//for now implemented as a char stack
//should be able to hold strings?
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

charStack::initStack(charStack *stackPtr, int maxsize) {
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

charStack::destroyStack(charStack *stackPtr){
	free(stackPtr->contents);
	stackPtr->contents = NULL;
	stackPtr->maxSize = 0;
	stackPtr->top = -1;
}

charStack::isEmpty(charStack *stackPtr){
	return (stackPtr->top < 0)
}

charStack::isFull(charStack *stackPtr){
	return (stackPtr->top >= stackPtr->maxSize - 1);
}

charStack::push(charStack *stackPtr, char element) {
	if (isFull(stackPtr)) {
		//stack is full
		//reallocate new stack with larger capacity
	}
	stackPtr->contents[++stackPtr->top] = element;
}

charStack::pop(charStack *stackPtr){
	if (isEmpty(stackPtr))
		return NULL;
	return stackPtr->contents[stackPtr->top--];
}