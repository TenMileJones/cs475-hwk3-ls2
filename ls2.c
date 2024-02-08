#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"

//TODO - difference between dir and dirent; make sure using readdir correctly to iterate through while loop

/**
 * TODO - comment
 * 
 * @return 0 if should not print, 1 if should print
*/
int ls2(stack_t* s, int runMode, char* pattern[], DIR* dirp, int* numIndents) {
    int shouldIPrint = 0;
    char* dirName = readdir(dirp)->d_name;
    if(runMode == 1)
        shouldIPrint = 1;
    while(dirp != NULL){
        
        //test if file or dir with help of lstat
        //char* filePath = (char*) malloc(1+)

        struct stat *fileinfo = (struct stat*) malloc(sizeof(struct stat));
        lstat(dirp, fileinfo);

        if(S_ISDIR(fileinfo->st_mode)){
            //exclude '.' and '..'
            if((strcmp(".", dirName) == 0) || strcmp("..", dirName) == 0){
                //skip this directory entry
                readdir(dirp);
                continue;
            }
            //if directory, recurse
            int printMe = ls2(s, runMode, pattern, opendir(dirName), numIndents+1);
            if(printMe){
                shouldIPrint = 1;
                
                //allocate space for dir string
                char dirLabel[] = " (directory)";
                char* directoryString = (char*) malloc(1+strlen(dirName)+strlen(dirLabel)+(sizeof(INDENT) * *numIndents));
		        directoryString[0] = '\0';
                
                //add indents and name
                addIndentsAndName(directoryString, numIndents, dirName);

                //add directory label
                strcat(directoryString, dirLabel);
                //push to stack
                push(s, directoryString);
            }
                
        } else if(S_ISREG(fileinfo->st_mode)){
            int patternMatches = FALSE;
            if(runMode == 2){
                //Check to see if pattern matches
                if(strcmp(pattern, dirName) == 0){
                    patternMatches = TRUE;
                }
            }

            if(runMode == 1 || patternMatches == TRUE){
                shouldIPrint = 1;
                //convert file size to string
                //repurposed from https://stackoverflow.com/questions/8257714/how-can-i-convert-an-int-to-a-string-in-c
                long fileSize = fileinfo->st_size;
                int fileSizeStringLength = snprintf(NULL, 0, "%ld", fileSize);
                char* fileSizeString = malloc(fileSizeStringLength+1);
                snprintf(fileSizeString, fileSizeStringLength + 1, "%ld", fileSize);

                //allocate space for file string (+10 for '( ', ' bytes)', and '/0')
                char* directoryString = (char*) malloc(10+strlen(dirName)+strlen(fileSizeString)+(sizeof(INDENT) * *numIndents));
                directoryString[0] = '\0';
                
                //add indents and name (directoryString is updated, serves as output param)
                addIndentsAndName(directoryString, numIndents, dirName);
                
                //add directory label
                strcat(directoryString, " (");
                strcat(directoryString, fileSizeString);
                strcat(directoryString, " bytes)");
                //push to stack
                push(s, directoryString);
                free(fileSizeString);
            }
        }
        readdir(dirp);
    }
    
    return shouldIPrint;
}


void addIndentsAndName(char* directoryString, int numIndents, char* dirName) {
    //add indents
    int i = numIndents-1;
    if(numIndents > 0){
        strcopy(directoryString, INDENT);
    }
    while(i>0){
        strcat(directoryString, INDENT);
    }
    //add directory name
    if(numIndents == 0){
        strcpy(directoryString, dirName);
    } else {
        strcat(directoryString, dirName);
    }
}

//remember: readdir() example code searches
// current directory for specific entry
