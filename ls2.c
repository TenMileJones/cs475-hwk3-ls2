#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"

/**
 * An overhauled version of the file listing command 'ls'. Prints all files and subdirectories recursively in mode 1,
 * or only ones that are relevant to pattern if in mode 2.
 * @param s stack_t that stores strings to be printed as output
 * @param runMode int MUST BE 1 or 2: 1 prints all files and subdirectories. 2 prints only files exactly matching pattern, 
 *      and its parent directories
 * @param pattern String pattern of file user wants to find in mode 2.
 * @param filePath String filepath of current level of recursion
 * @param numIndents int number of indentations, equal to number of recursive calls. Should start at 0.
 * @return 0 if should not print, 1 if should print
*/
int ls2(stack_t* s, int runMode, char* pattern, char* filePath, int numIndents) {
    int shouldIPrint = 0;
    if(runMode == 1)
        shouldIPrint = 1;

    DIR* dirp = opendir(filePath);
    struct dirent* readResult = readdir(dirp);
    char* dirName;
    while(TRUE){
        if(readResult != NULL){
            dirName = readResult->d_name;
        } else { 
            //new dirent is null, therefore end of this directory
            closedir(dirp);
            return shouldIPrint;
        }
        break;
    }
    
    while(readResult != NULL){
        //malloc and concatenate new filePath
        char* newFilePath = (char*) malloc(2+strlen(filePath)+strlen(dirName));
        newFilePath[0] = '\0';
        strcpy(newFilePath, filePath);
        strcat(newFilePath, "/");
        strcat(newFilePath, dirName);
        
        struct stat *fileinfo = (struct stat*) malloc(sizeof(struct stat));
        lstat(newFilePath, fileinfo);

        if(S_ISDIR(fileinfo->st_mode)){
            
            //exclude '.' and '..'
            if(isForbiddenDirName(dirName)){
                //skip this directory entry
                readResult = readdir(dirp);
                free(newFilePath);
                free(fileinfo);
                if(readResult != NULL){
                    dirName = readResult->d_name;
                } else {
                    //new dirent is null, therefore end of this directory
                    break;
                }
                continue;
            }
            //if directory, recurse
            int printMe = ls2(s, runMode, pattern, newFilePath, numIndents+1);
            if(printMe){
                shouldIPrint = 1;
                
                //allocate space for dir string
                char dirLabel[] = " (directory)";
                char* directoryString = (char*) malloc(1+strlen(dirName)+strlen(dirLabel)+(strlen(INDENT) * numIndents));
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
                fileSizeString[0] = '\0';
                snprintf(fileSizeString, fileSizeStringLength + 1, "%ld", fileSize);

                //allocate space for file string (+10 for ' (', ' bytes)', and '/0')
                char* directoryString = (char*) malloc(10+strlen(dirName)+strlen(fileSizeString)+(strlen(INDENT) * numIndents));
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
        readResult = readdir(dirp);
        if(readResult != NULL){
            dirName = readResult->d_name;
        }
        free(fileinfo);
        free(newFilePath);
    }
    closedir(dirp);
    return shouldIPrint;
}

/**
 * Helper method for ls2. Creates the first part of the output string for a given file that will be passed to the output stack.
 * @param directoryString pointer to empty string to be added to. Serves as output parameter
 * @param numIndents int number of indentations to concatenate to start of directoryString
 * @param dirName String name of directory to be concatenated after indentations
*/
void addIndentsAndName(char* directoryString, int numIndents, char* dirName) {
    //add indents
    int i = numIndents-1;
    if(numIndents > 0){
        strcpy(directoryString, INDENT);
    }
    while(i>0){
        strcat(directoryString, INDENT);
        i--;
    }
    //add directory name
    if(numIndents == 0){
        strcpy(directoryString, dirName);
    } else {
        strcat(directoryString, dirName);
    }
}

/**
 * Helper method for ls2. Determines if this directory name is forbidden/should be ignored by ls2.
 * @param dirName String name of directory
 * @return 1 if directory name is forbidden, 0 otherwise
*/
int isForbiddenDirName(char* dirName) {
    if((strcmp(".", dirName) == 0) || (strcmp("..", dirName) == 0)){
        return TRUE;
    }
    return FALSE;
}
