//
// Created by Jenny on 2017-10-09.
//
#include"testProgram.h"
//list *ls;
int main(void){
    testCreateNewList();
    testSizeOfNewList();
    testAddValue();
    testIsEmpty();
    testGetValueFromIndex();
    testRemoveNode();
    testRemoveNode2();
    testGetFirstNode();
    testGetFirstNodeOneItem();
    fprintf(stderr,"All tests passed\n");
    return 0;
}
void testCreateNewList(){
    list *ls = newEmptyLinkedList();
    assert(ls!=NULL);
    free(ls);
}
void testSizeOfNewList(){
    list *ls = newEmptyLinkedList();
    assert(ls->size==0);
    free(ls);
}
void testAddValue(){
    list *ls = newEmptyLinkedList();
    int firstElement = 4;
    int *pointer = &firstElement;
    node *node = malloc(sizeof(node));
    node->data = pointer;
    addValue(ls,node);
    assert(ls->size==1);
    free(ls);
    free(node);
}
void testIsEmpty(){
    list *ls = newEmptyLinkedList();
    assert(isEmpty(ls));
    free(ls);
}

void testGetValueFromIndex(){
    list *ls = newEmptyLinkedList();
    int firstElement;
    int *pointer = &firstElement;
    *pointer = 4;
    int secondElement;
    int *pointer2 = &secondElement;
    *pointer2 = 6;
    node *firstNode = malloc(sizeof(node));
    node *secondNode = malloc(sizeof(node));
    firstNode->data = pointer;
    secondNode->data = pointer2;
    addValue(ls,firstNode);
    addValue(ls,secondNode);

    node *returnNode = getNodeFromIndex(ls,0);
    int returnValue = *(int *)returnNode->data;
    assert(returnValue==6);
    free(ls);
}

void testRemoveNode(){
    int numberOfNodes = 4;
    list *ls = newEmptyLinkedList();
    node *tempNodeFirst;
    node *tempNodeSecond;
    for (int i = 0; i <numberOfNodes ; ++i) {
        node *newNode = calloc(sizeof(node),1);
        int *tempP = calloc(sizeof(int),1);
        *tempP = i;
        newNode->data = tempP;
        addValue(ls,newNode);
    }
    tempNodeFirst=getNodeFromIndex(ls, 0);
    removeNodeFromList(tempNodeFirst, ls);
    tempNodeSecond = getNodeFromIndex(ls,0);
    assert(ls->size == 3);
    assert( tempNodeSecond == ls->next);
}

void testRemoveNode2(){
    int numberOfNodes = 4;
    list *ls = newEmptyLinkedList();
    node *tempNodeFirst;
    node *tempNodeSecond;
    for (int i = 0; i <numberOfNodes ; ++i) {
        node *newNode = calloc(sizeof(node),1);
        int *tempP = calloc(sizeof(int),1);
        *tempP = i;
        newNode->data = tempP;
        addValue(ls,newNode);
    }
    tempNodeFirst=getNodeFromIndex(ls,2);
    removeNodeFromList(tempNodeFirst, ls);
    tempNodeSecond = getNodeFromIndex(ls,2);
    int i = *(int*)tempNodeSecond->data;
    assert(ls->size == 3);
    assert(i == 0);
}

void testGetFirstNode(){
    int numberOfNodes = 4;
    list *ls = newEmptyLinkedList();
    node *tempNodeFirst;
    node *tempNodeSecond;
    for (int i = 0; i <numberOfNodes ; ++i) {
        node *newNode = calloc(sizeof(node),1);
        int *tempP = calloc(sizeof(int),1);
        *tempP = i;
        newNode->data = tempP;
        addValue(ls,newNode);
    }
    tempNodeFirst=getNodeFromIndex(ls,0);
    int tempValueFirst = *(int*)tempNodeFirst->data;
    tempNodeSecond = getFirstNode(ls);
    removeNodeFromList(tempNodeSecond,ls);
    int tempValueSecond = *(int*)tempNodeSecond->data;
    assert(tempValueFirst == tempValueSecond);
    assert(ls->size == 3);
}
void testGetFirstNodeOneItem(){
    list *ls = newEmptyLinkedList();
    node *newNode = calloc(sizeof(node),1);
    int *tempP = calloc(sizeof(int),1);
    *tempP = 01;
    newNode->data = tempP;
    addValue(ls,newNode);

    node *tempNodeFirst;
    node *tempNodeSecond;
    tempNodeFirst=getNodeFromIndex(ls,0);
    int tempValueFirst = *(int*)tempNodeFirst->data;
    tempNodeSecond = getFirstNode(ls);
    int tempValueSecond = *(int*)tempNodeSecond->data;
    assert(tempValueFirst == tempValueSecond);
    assert(isEmpty(ls));
}