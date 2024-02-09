#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "stack.h"
#include "ls2.h"

/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 */
int main(int argc, char* argv[]) {
	argc -= 1;
	// stack stores the lines to print out
	if((argc != 1) && (argc != 2)){
		printf("Improper usage: ./ls2 <path> [exact-match-pattern]\n");
		return 0;
	}
	DIR* dirp = opendir(argv[1]);
	if(dirp == NULL){
		printf("%s is not a directory path! Exiting...\n", argv[1]);
		return 0;
	}
	stack_t *s = initstack();
	char *pattern;
	if(argc == 1){
		ls2(s, argc, NULL, argv[1], 0);
	} else {
		pattern = (char*) malloc(strlen(argv[2])+1);
		pattern[0] = '\0';
		strcpy(pattern, argv[2]);
		ls2(s, argc, pattern, argv[1], 0);
		free(pattern);
	}
	closedir(dirp);

	// print stack
	printstack(s);

	// free up stack
	freestack(s);
	return 0;
}
