/* <mfind>.c
 *  <Jenny> <Kinert>
 *  Laboration 4 <mfind> <5DV088> <HT17>
 *  <A search program similar to the command find>
 *  <The user enters filetype, number of threads to be created, files to search>
 *  <and the name to search for>
 */
#include "mfind.h"
list *ls = NULL;
int threadCounter = 0;
programInput *pi;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

int main(int argc,char** argv){
    programInput *pi;
    ls = newEmptyLinkedList();
    pi = extractProgramInput(argc,argv);
    pi->name = argv[argc-1];
    addDirectoriesToList(argv,argc,pi->numberOfDirectories);
    createThreads(pi);
    free(pi);
    free(ls);
    return 0;
}

/**
 * Name: CreateThreads
 * Description: Creates number of threads indicated from the user.
 * @param pi
 */
void createThreads(programInput *pi){
    pthread_t threads[pi->numberOfThreads];
    for (long i = 0; i < pi->numberOfThreads-1; ++i) {
        int rc = pthread_create(&threads[i], NULL, threadMain,(void *)i);
        if (rc){
            fprintf(stderr,"ERROR; return code from pthread_create() is %d\n",
                    rc);
            exit(-1);
        }
    }
    long temp2 = pi->numberOfThreads-1;
    void *temp = (void *)temp2;
    threadMain(temp);

    for (long j = 0; j < pi->numberOfThreads-1 ; ++j) {
        if(pthread_join(threads[j], NULL)) {
            fprintf(stderr, "Error joining thread\n");
            exit(1);
        }
    }
}
/**
 * Name: threadMain
 * Description: This is the thread part of the program. CreateThreds redirects
 * to this function.
 * @param threadid
 * @return
 */
void *threadMain(void *threadid) {
    long thread = (long)threadid;
    int directoryCounter=0;
    struct dirent *pDirent;
    node *firstNode;
    DIR *searchDir;
    while(threadCounter != pi->numberOfThreads){
        pthread_mutex_lock(&mut);
        if(isEmpty(ls)){
            threadCounter++;
            if(threadCounter == pi->numberOfThreads){
                pthread_cond_broadcast(&cond);
                pthread_mutex_unlock(&mut);
                break;
            }
            pthread_cond_wait(&cond,&mut);
            threadCounter--;
            pthread_mutex_unlock(&mut);
        }
        else{
            firstNode = getFirstNode(ls);
            pthread_mutex_unlock(&mut);
            char *searchFile = firstNode->data;
            if((searchDir = opendir(searchFile))== NULL){
                perror(searchFile);
            }
            else{
                directoryCounter++;
                while ((pDirent = readdir(searchDir)) != NULL) {
                    char *newDirectory = getNewDirectory(pDirent->d_name,searchFile);
                    controlIfCorrectSearchname(pDirent->d_name,newDirectory);
                    if(controlIfDirectory(newDirectory) &&
                       controlIfValidDirectory(pDirent->d_name)){
                        node *newNode= calloc(1,sizeof(node));
                        controlMemoryAllocated(newNode);

                        newNode->data = newDirectory;
                        pthread_mutex_lock(&mut);
                        addValue(ls,newNode);
                        pthread_cond_signal(&cond);
                        pthread_mutex_unlock(&mut);
                    }
                    else{
                        free(newDirectory);
                    }
                }
            }
            closedir(searchDir);
            free(firstNode->data);
            free(firstNode);
        }
    }
    fprintf(stderr,"Thread: %ld Reads: %d\n",thread,directoryCounter);
    return 0;
}

/**
 * Name: getNewDirectory
 * Description: Takes input in form of the name of directory thats entered and
 * the startdirectory from which the program is based. They are then putted
 * togheter and returned so the whole path to the directory can be saved.
 * @param directoryName
 * @param startDirectory
 * @return
 */
char *getNewDirectory(char directoryName[], char *startDirectory){
    int lengthOfBuff = 0;
    lengthOfBuff = strlen(directoryName);
    lengthOfBuff += strlen(startDirectory);
    char *newDirectory = calloc(sizeof(char),(lengthOfBuff+2));
    if(newDirectory == NULL){
        fprintf(stderr, "ERROR: %s\n", newDirectory);
        exit(1);
    }
    strncpy(newDirectory,startDirectory,strlen(startDirectory));
    strncat(newDirectory,"/",1);
    strncat(newDirectory, directoryName,strlen(directoryName));
    strncat(newDirectory,"\0",1);
    return newDirectory;
}

/**
 * Name: controlIfDirectory
 * Description: Controls if an item found in a directory is a new directory.
 * @param newDirectory
 * @return true or false
 */
bool controlIfDirectory(char *newDirectory){
    struct stat fileStat;
    if(lstat(newDirectory,&fileStat)==-1){
        return false;
    }
    else if(S_ISDIR(fileStat.st_mode)!=0){
        return true;
    }
    return false;
}
/**
 * Name: controlIfValidDirectory
 * Description: Controls if the directory to be added is not the directory . or
 * ..
 * @param newDirectory
 * @return true or false
 */
bool controlIfValidDirectory(char *newDirectory){
    if(newDirectory[0] == '.' && strlen(newDirectory) < 2){
        return false;
    }
    else if(newDirectory[0] == '.' && strlen(newDirectory) == 2){
        if(newDirectory[1] == '.'){
            return false;
        }
    }
    return true;
}

/**
 * Name: extractProgramInput
 * Description: Controls what preferences the user has given to the program.
 * Filetype and number of threads is saved in a struct for later access.
 * @param argc
 * @param argv
 * @return
 */
programInput *extractProgramInput(int argc, char** argv){
    pi = calloc(sizeof(programInput),1);
    if(pi == NULL){
        fprintf(stderr, "ERROR: Could not allocate programInput struct");
        exit(1);
    }
    char option =0;
    extern int optind;
    int numberOfThreads = 0;
    extern char *optarg;
    while ((option = getopt(argc,argv,"t:p:")) !=-1){
        switch(option){
            case('t') :
                if(strcmp(optarg,"d")==0 || strcmp(optarg,"l") == 0 ||
                        strcmp(optarg,"f") == 0){
                    pi->filetype = optarg;
                }
                else{
                    fprintf(stderr,"Error: Invalid type\n");
                    exit(1);
                }
                break;
            case('p') :
                if(isdigit(*optarg)){
                    numberOfThreads=atoi(optarg);
                    pi->numberOfThreads = numberOfThreads;
                   break;
                }
                else{
                    fprintf(stderr,"Error: number of threads is not a number\n");
                    exit(1);
                }
            default: printf("something went wrong\n");
                exit(EXIT_FAILURE);
        }
    }
    if(numberOfThreads == 0){
        pi->numberOfThreads = 1;
    }
    pi->numberOfDirectories = optind;
    return pi;
}

/**
 * Name: addDirectoriesToList
 * Description: Adds the directories given by the user to the list.
 * @param argv
 * @param argc
 * @param index
 */
void addDirectoriesToList(char **argv, int argc, int index){
    int numberOfDirectories = argc - index - 1;
    if(numberOfDirectories == 0){
        fprintf(stderr, "No directories where added as startFile \n");
        exit(1);
    }
    for (int i = 0; i < numberOfDirectories; ++i) {
        if(controlIfDirectory(argv[index+i])){
            if(controlIfValidDirectory(argv[index +i])){
                controlIfCorrectSearchname(argv[index +i],argv[index +i]);
            }
        }
        node *newNode = calloc(sizeof(node),1);
        controlMemoryAllocated(newNode);
        if((newNode->data = strdup(argv[index+i]))==NULL){
            perror("strdup");
            exit(1);
        }
        addValue(ls, newNode);
    }
}

/**
 * Name: controlIfCorrectSearchname
 * Description: Controls if the item searched is the same as the user indicated
 * in the beginning and if the filetype is correct.
 * @param filename
 * @param newDirectory
 */
void controlIfCorrectSearchname(char *filename, char *newDirectory){
    struct stat fileStat;
    char *baseName = NULL;
    baseName = basename(filename);
    if(strcmp(baseName,pi->name)==0){
        if(pi->filetype != NULL){
            if(lstat(newDirectory,&fileStat)==-1){
                perror("lstat: ");
                exit(1);
            } else if(S_ISREG(fileStat.st_mode)!=0 &&
                    strcmp(pi->filetype,"f")==0){
                printf("%s\n",newDirectory);
            } else if(S_ISLNK(fileStat.st_mode)!=0 &&
                    strcmp(pi->filetype,"l")==0){
                printf("%s\n",newDirectory);
            }else if(S_ISDIR(fileStat.st_mode)!=0 &&
                    strcmp(pi->filetype,"d")==0){
                printf("%s\n",newDirectory);
            }
        }
        else{
            printf("%s\n",newDirectory);
        }
    }
}
/**
 * Name controlMemoryAllocated
 * Description controls if function systemcalls was allocated correctly
 * @param str
 */
static void controlMemoryAllocated(node *n) {
    if(n == NULL){
        fprintf(stderr, "ERROR: node was not allocated\n");
        exit(EXIT_FAILURE);
    }
}
