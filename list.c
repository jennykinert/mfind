//
// Created by Jenny on 2017-07-27.
//
#include "list.h"
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
/**
 * Name: newEmptyLinkedList
 * Description: Creates a new empty linked list
 * @return: Pointer to the List
 */
list *newEmptyLinkedList(){
    list* list1 = malloc(sizeof(list));
    listSystemCheck(list1);
    list1->size=0;
    list1->next=NULL;
    return list1;
}

/**
 * Name: addValue
 * Description: Adds a new value to the list
 * @param list
 * @param ui
 */
void addValue(list *list, node *node){
    pthread_mutex_lock(&mutex);
    node->next = list->next;
    list->next = node;
    list->size++;
    pthread_mutex_unlock(&mutex);
}

/**
 * Name: removeFirstValue
 * Description: Redirect the first value in the list to the second value and
 * return the value to be deleted
 * @param list
 */
void removeNodeFromList(node *removeNode,list *ls){
    node *secondNode = malloc(sizeof(node));
    node *firstNode = malloc(sizeof(node));
    bool deletedNode = false;
    firstNode = ls->next;
    secondNode->data = NULL;
    while(firstNode != NULL){
        if(firstNode == removeNode){
            secondNode->next = firstNode->next;
            if(firstNode == ls->next){
                ls->next = firstNode->next;
            }
            firstNode=NULL;
            free(firstNode);
            ls->size--;
            deletedNode = true;
            break;
        }
        secondNode = firstNode;
        firstNode = firstNode->next;
    }
    if(!deletedNode){
        fprintf(stderr, "Node do not exist in list\n");
    }
}

/**
 *
 * @param list
 * @return
 */
node *getFirstNode(list *list){
    pthread_mutex_lock(&mutex);
    node *node = list->next;
    removeNodeFromList(node,list);
    pthread_mutex_unlock(&mutex);
    return node;
}

/**
 * Name: isEmpty
 * Description: controls if the list is empty or not
 * @param:
 * @return: boolean
 */
bool isEmpty(list *list){
    pthread_mutex_lock(&mutex);
    if(list->size==0){
        pthread_mutex_unlock(&mutex);
        return true;
    }
    else{

        pthread_mutex_unlock(&mutex);
        return false;
    }
}

/**
 * Name: sizeOfList
 * Description: Controls the size of the list and return the value
 * @param list
 * @return size
 */
int sizeOfList(list *list){
    pthread_mutex_lock(&mutex);
    int size = list->size;
    pthread_mutex_unlock(&mutex);
    return size;
}

/**
 * Name: swap
 * Description: Function to help swapping elements when sorting the list with
 * bubblesort
 * @param list
 */
/**void swap(node *element1, node *element2){
    node *temp = element1;
    element1->next = element2->next;
    element2->next=temp;
}**/

/**
 * Name: getNodeFromIndex
 * Description: Resturns the user info positioned at the specified index as a
 * pointer
 * @param list
 * @param index
 * @return *ui (The user info at the index)
 */
node *getNodeFromIndex(list *list, int index){
    pthread_mutex_lock(&mutex);
    node *node = list->next;
    unsigned int temp = index;
    if(temp >= list->size){
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    for(int i = 0; i<index ; i++){
        node = node->next;
    }
    pthread_mutex_unlock(&mutex);
    return node;
}

/**
 * Name: sortList
 * Description: Sorts the list according to the algorithm bubblesort
 * @param list
 */
/**void sortList(list *list){
    node *first= list->next;

    while(first != NULL ){
        node *second = first->next;
        while(second != NULL){
            bool control = list->compare(first,second);
            if(control){
                swap(first, second);
            }
            second = second->next;
        }
        first=first->next;
    }
}**/

/**
 * This function is only part of the implementation
 * and not of the interface. Used for checking of malloc returns
 */
void listSystemCheck(void *memory){
    if(memory==NULL){
        perror("Error in list listSystemCheck: ");
        exit(EXIT_FAILURE);
    }
}





