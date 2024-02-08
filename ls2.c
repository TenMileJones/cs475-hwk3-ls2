#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"


// TODO: function definitions here for ls2

/**
 * This is just an example. Delete this before 
 * submission.
 */
void example(int* x) {
    *x = thisIsGlobal;
} 

/**
 * TODO - comment
 * 
 * @return 0 if should not print, 1 if should print
*/
int ls2(stack_t* s, int* runMode, char* pattern[], DIR* dirp, int* numIndents) {
    int shouldIPrint = 0;
    if(runMode == 1)
        shouldIPrint = 1;
    while(dirp){

        char* dirName = getNameFromDirent(readdir(dirp));
        
        //test if file or dir with help of lstat
        struct stat *fileinfo = (struct stat*) malloc(sizeof(struct stat));
        lstat(dirp, fileinfo);

        if(S_ISDIR(fileinfo->st_mode)){
            int printMe = ls2(s, runMode, pattern, opendir(dirName), numIndents+1);
            if(printMe){
                shouldIPrint = 1;
                
                //allocate string for dir
                char dirLabel[] = " (directory)";
                char* stackDir = (char*) malloc(1+strlen(dirName)+strlen(dirLabel)+(sizeof(INDENT) * *numIndents));
		        stackDir[0] = '\0';
                
                //add indents
                int i = numIndents-1;
                if(numIndents > 0){
                    strcopy(stackDir, INDENT);
                }
                while(i>0){
                    strcat(stackDir, INDENT);
                }
                //add directory name
                if(numIndents == 0){
                    strcpy(stackDir, dirName);
                } else {
                    strcat(stackDir, dirName);
                }
                //add directory label
                strcat(stackDir, dirLabel);
                //push to stack
                push(s, stackDir);
            }
                
        } else if(S_ISREG(fileinfo->st_mode)){
            
        }

        
    }
    
    return shouldIPrint;
}

char* getNameFromDirent(struct dirent* dirp) {
    //TODO - is param ok?

    return 'A';
}

//remember: readdir() example code searches
// current directory for specific entry
