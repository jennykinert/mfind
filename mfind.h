#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "list.h"

typedef struct programInput{int numberOfThreads; int numberOfDirectories;
    char *filetype;}
programInput;
void createThreads(int numberOfThreads);

void addValueToList(void *threadid);
programInput extractProgramInput(int argc, char** argv);

/**
 * Name: freeListItems
 * Description: frees the memory of the lists allocated items
 * @param ls: list to be freed
 */
void freeListItems(list *ls);

bool controlIfDirectory(char *newDirectory);

char *getNewDirectory(char directoryName[], char *startDirectory);

void *threadMain(void *threadid);

bool controlIfValidDirectory(char *newDirectory);

void addDirectoriesToList(char **argv, int argc, int index);