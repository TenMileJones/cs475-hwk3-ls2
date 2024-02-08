#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "ls2.h"

/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 */
int main(int argc, char* argv[]) {
	// stack stores the lines to print out
	if((argc != 1) && (argc != 2)){
		printf("./ls2 <path> [exact-match-pattern]\n");
		return 0;
	}
	void* dirp = opendir(argv[1]);
	if(dirp == NULL){
		printf("%s is not a directory path! Exiting...\n", argv[1]);
		return 0;
	}
	
	
	stack_t *s = initstack();

	
	
	
	push(s, "Hello1");
	push(s, "Hello2");
	push(s, "Hello3");

	// print stack
	printstack(s);

	// free up stack
	freestack(s);
	return 0;
}
