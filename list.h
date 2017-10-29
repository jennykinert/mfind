/* <list>.h
 *  <Jenny> <Kinert>
 *  Laboration 4 <mfind> <5DV088> <HT17>
 *  <A simple linked list >
 */
#ifndef LINKEDLIST_LIST_H
#define LINKEDLIST_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

/**
 * Name: list
 * Description: This is the head of the list. which keep track of the list size
 * and the first element of the list
 * @param size (the number of elements in the list)
 * @parm next (pointer to an element in this list)
 */
typedef struct head{ unsigned int size; void* next;} list;

/**
 * Name: node
 * Description: The node holds the data items in the list and points to the
 * next node in the list.
 */
typedef struct node{void *data; struct node *next;} node;

/**
 * Function: newEmptyLinkedList
 * Description: Creates a new empty linked list
 * @return: Pointer to the List
 */
list *newEmptyLinkedList();

/**
 * Name: addValue
 * Description: Adds a new value to the list
 * @param list
 * @param ui
 */
void addValue(list *list, node *node);

/**
 * Name: removeFirstValue
 * Description: Redirect the first value in the list to the second value and
 * return the value to be deleted
 * @param list
 */
void removeNodeFromList(node *node,list *ls);

/**
 * Name: isEmpty
 * Description: controls if the list is empty or not
 * @param: list
 * @return: boolean
 */
bool isEmpty(list *list);

/**
 * Name: sizeOfList
 * Description: Controls the size of the list and return the value
 * @param list
 * @return size
 */
int sizeOfList(list *list);

/**
 * Name: swap
 * Description: Function to help swapping elements when sorting the list with
 * bubblesort
 * @param list
 */
void swap(node *element1, node *element2);

/**
 * Name: getUserInfoFromIndex
 * Description: Resturns the user info positioned at the specified index
 * @param list
 * @param index
 * @return
 */
node *getNodeFromIndex(list *list, int index);

/**
 * Name: sortList
 * Description: Sorts the list according to the algorithm bubble sort
 * @param list
 */
void sortList(list *list);

/**
* Name: systemmCheck
* This function is only part of the implementation
* and not of the interface.
*/
void listSystemCheck(void *memory);

/**
 * Name: getFirstNode
 * Description: Returns the first node of the list and after deleting it from
 * the list.
 * @param list
 * @return
 */
node *getFirstNode(list *list);


#endif //LINKEDLIST_LIST_H

