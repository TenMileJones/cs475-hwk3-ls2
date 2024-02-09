#ifndef LS2_H_INCLUDED
#define LS2_H_INCLUDED

#include "stack.h"

#define TRUE 1
#define FALSE 0
#define INDENT "    "


int ls2(stack_t* s, int runMode, char* pattern, char* filePath, int numIndents);
void addIndentsAndName(char* directoryString, int numIndents, char* dirName);
int isForbiddenDirName(char* dirName);

#endif
