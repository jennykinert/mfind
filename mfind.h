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
#include <libgen.h>
#include "list.h"

/**
 * Name: programInput
 * Description: A struct with necessities to the program
 */
typedef struct programInput{int numberOfThreads; int numberOfDirectories;
    char *filetype; char *name;}
programInput;

/**
 * Name: CreateThreads
 * Description: Creates number of threads indicated from the user.
 * @param pi
 */
void createThreads(programInput *pi);

/**
 * Name: extractProgramInput
 * Description: Controls what preferences the user has given to the program.
 * Filetype and number of threads is saved in a struct for later access.
 * @param argc
 * @param argv
 * @return
 */
programInput *extractProgramInput(int argc, char** argv);

/**
 * Name: controlIfDirectory
 * Description: Controls if an item found in a directory is a new directory.
 * @param newDirectory
 * @return true or false
 */
bool controlIfDirectory(char *newDirectory);

/**
 * Name: getNewDirectory
 * Description: Takes input in form of the name of directory thats entered and
 * the startdirectory from which the program is based. They are then putted
 * togheter and returned so the whole path to the directory can be saved.
 * @param directoryName
 * @param startDirectory
 * @return
 */
char *getNewDirectory(char directoryName[], char *startDirectory);

/**
 * Name: threadMain
 * Description: This is the thread part of the program. CreateThreds redirects
 * to this function.
 * @param threadid
 * @return
 */
void *threadMain(void *threadid);

/**
 * Name: controlIfValidDirectory
 * Description: Controls if the directory to be added is not the directory . or
 * ..
 * @param newDirectory
 * @return true or false
 */
bool controlIfValidDirectory(char *newDirectory);

/**
 * Name: addDirectoriesToList
 * Description: Adds the directories given by the user to the list.
 * @param argv
 * @param argc
 * @param index
 */
void addDirectoriesToList(char **argv, int argc, int index);

/**
 * Name: controlIfCorrectSearchname
 * Description: Controls if the item searched is the same as the user indicated
 * in the beginning and if the filetype is correct.
 * @param filename
 * @param newDirectory
 */
void controlIfCorrectSearchname(char *filename, char *newDirectory);

/**
 * Name controlMemoryAllocated
 * Description controls if function systemcalls was allocated correctly
 * @param str
 */
static void controlMemoryAllocated(node *n);