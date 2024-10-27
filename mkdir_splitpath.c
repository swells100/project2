
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){

    if (strlen(pathName) == 0) {
        printf("MKDIR ERROR: no path provided/n");
        return;
    }

    char baseName[64];
    char dirName[64];

    struct NODE* parentDir = splitPath(pathName, baseName, dirName);
    if (parentDir == NULL) {
        return;
    }

    struct NODE* child = parentDir->childPtr;
    while (child != NULL) {
        if (strcmp(child->name, baseName) == 0) {
            printf("MKDIR ERROR: directory %s already exists\n", baseName);
            return;
        }
    }

    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newDir->name, baseName);
    newDir->fileType = 'D';
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;
    newDir->parentPtr = parentDir;
    

    if (parentDir->childPtr == NULL) {
        parentDir->childPtr = newDir;
    } else {
        struct NODE* lastChild = parentDir->childPtr;
        while (lastChild->siblingPtr != NULL) {
            lastChild = lastChild->siblingPtr;
        }
        lastChild->siblingPtr = newDir;
    }

    printf("MKDIR SUCCESS: node %s succesffully created\n", pathName);

    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    char* lastSlash = strrchr(pathName, '/');
    if (lastSlash != NULL) {
        size_t dirLen = lastSlash - pathName;
        strncpy(dirName, pathName, dirLen);
        dirName[dirLen] = '\0';

        strcpy(baseName, lastSlash + 1);
    } else {
        strcpy(dirName, "");
        strcpy(baseName, pathName);
    }

    struct NODE* current = root;
    char* token = strtok(dirName, "/");
    
    while (token != NULL) {
        struct NODE* found = NULL;
        struct NODE* child = current->childPtr;

        while (child != NULL) {
            if (strcmp(child->name, token) == 0) {
                found = child;
                break;
            }
            child = child->siblingPtr;
        }
    

      if (found == NULL) {
          printf("ERROR: directory %s does not exist\n", token);
          return NULL;
      }

      current = found;
      token = strtok(NULL, "/");
    }

    return current;
}
