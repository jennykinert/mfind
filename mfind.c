#include "mfind.h"
//#define numberOfThreads1 910
list *ls=NULL;
int threadCounter = 0; //TODO räkna antalet trådar som väntar om man är sista tråden döda alla gör det med condition variabeln.
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

//TODO ta reda på om varje tråd kan ha en egen struct som den använder.
int main(int argc,char** argv){
    programInput pi;
    ls = newEmptyLinkedList();
    pi = extractProgramInput(argc,argv);
    addDirectoriesToList(argv,argc,pi.numberOfDirectories);
    createThreads(pi.numberOfThreads);
    return 0;
}

void createThreads(int numberOfThreads){
    pthread_t threads[numberOfThreads];
    for (long i = 0; i < numberOfThreads-1; ++i) {
        int rc = pthread_create(&threads[i], NULL, threadMain,(void *)i);
        if (rc){
            fprintf(stderr,"ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    void *temp = (void *)numberOfThreads-1;
    threadMain(temp);

    for (long j = 0; j < numberOfThreads-1 ; ++j) {
        if(pthread_join(threads[j], NULL)) {
            fprintf(stderr, "Error joining thread\n");
            exit(1);
        }
    }
}

void *threadMain(void *threadid) {
    struct dirent *pDirent;
    DIR *searchDir;
    pthread_mutex_lock(&mut);
    //while(threadCounter != )
    if(isEmpty(ls)){
        pthread_cond_wait(&cond,&mut);

        //TODO gör någonting här, använda wait men den tar in en mutex också vilket jag inte har i den här filen
    }
    else{
        node *firstNode = getFirstNode(ls);
        char *searchFile = (char *)firstNode->data;

        if((searchDir = opendir(searchFile))== NULL){
            perror("opendir: ");
            exit(1);
        }

        while ((pDirent = readdir(searchDir)) != NULL) {
            if(strcmp(pDirent->d_name,searchFile) == 0){
                printf("%s\n",pDirent->d_name);
            }


            char *newDirectory = getNewDirectory(pDirent->d_name,searchFile);
            if(controlIfDirectory(newDirectory) && controlIfValidDirectory(pDirent->d_name)){
                node *newNode= calloc(sizeof(node),1);
                newNode->data = newDirectory;
                addValue(ls,newNode);
                printf("Add Directory to List %s\n",pDirent->d_name);
            }
            else{
                printf("No directory did not add to List %s\n",pDirent->d_name);
            }
            printf ("content in pDirent: [%s]\n", pDirent->d_name);

        }
        closedir (searchDir);
    }
    for (int i = 0; i <ls->size ; ++i) {
        node *printNode = getNodeFromIndex(ls,i);
        fprintf(stderr,"%s\n",(char*)printNode->data);
    }
    return 0;
}
char *getNewDirectory(char directoryName[], char *startDirectory){
    int lengthOfBuff = 0;

    lengthOfBuff = strlen(directoryName);
    lengthOfBuff += strlen(startDirectory);
    char *newDirectory= calloc(sizeof(char),(lengthOfBuff+2));

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
programInput extractProgramInput(int argc, char** argv){
    programInput pi;
    char option =0;
    extern int optind;
    //node *newNode;
    int numberOfThreads = 0;
    extern char *optarg;
    while ((option = getopt(argc,argv,"t:p:")) !=-1){
        switch(option){
            case('t') :
                if(strcmp(optarg,"d")==0 || strcmp(optarg,"l") == 0 ||
                        strcmp(optarg,"f") == 0){
                    pi.filetype = optarg;
                    printf("filetype: %s\n",pi.filetype);
                }
                else{
                    fprintf(stderr,"Error: Invalid type");
                    exit(1);
                }
                break;
            case('p') :
                if(isdigit(*optarg)){
                    numberOfThreads=atoi(optarg);
                    pi.numberOfThreads = numberOfThreads;
                    printf("number of threads: %d\n",numberOfThreads);
                   break;
                }
                else{
                    fprintf(stderr,"Error: number of threads is not a number");
                    exit(1);
                }
            default: printf("something went wrong\n");
                exit(EXIT_FAILURE);
        }
    }
    pi.numberOfDirectories = optind;
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
    int numberOfDirectories = argc - index;
    for (int i = 0; i < numberOfDirectories; ++i) {
        node *newNode = calloc(sizeof(node),1);
        newNode->data = argv[index];
        addValue(ls, newNode);
    }
}



/**
 * Name: freeListItems
 * Description: frees the memory of the lists allocated items
 * @param ls: list to be freed
 */
void freeListItems(list *ls){
    while(ls->next!=NULL){
        node *freenode = ls->next;
        node *tempNode = freenode->next;
        free(freenode->data);
        free(freenode);
        ls->next = tempNode;
    }
}