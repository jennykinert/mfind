#include "stack.h"

struct node *top = NULL;

int isempty() {
    if(top == NULL)
        return 1;
    else
        return 0;
}
   
/**int isfull() {
    if(top == MAXSIZE)
        return 1;
    else
        return 0;
}**/

node peek() {
    return top;
}

int pop() {
    int data;  
    if(!isempty()) {
        data = stack[top];
        top = top - 1; 
            return data;
    } else {
        printf("Could not retrieve data, Stack is empty.\n");
        return 0; 
    }

}

void push(int data) {
    if(!isfull()) {
        top = top + 1;   
        stack[top] = data;
    } else {
        printf("Could not insert data, Stack is full.\n");
    }
}
    
