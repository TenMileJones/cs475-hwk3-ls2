#ifndef LS2_H_INCLUDED
#define LS2_H_INCLUDED

#include "stack.h"

#define TRUE 1
#define FALSE 0
#define INDENT "    "

// TODO: Any global variables go below

// TODO: Function declarations go below
int ls2(stack_t* s, int runMode, char* pattern[], DIR* dirp, int* numIndents);
void addIndentsAndName(char* directoryString, int numIndents, char* dirName);

#endif
