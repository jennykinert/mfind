#include <stdio.h>
typedef struct node { int data; struct node *next; }node;

int isempty();

int isfull();

node peek();

int pop();

void push(int d);
