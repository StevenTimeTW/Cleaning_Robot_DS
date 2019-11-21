//
//  Stack.cpp
//  Stack_Test
//
//  Created by Steven on 2019/11/15.
//  Copyright © 2019 Steven. All rights reserved.
//

#include "Stack.hpp"
#include <iostream>


void StackArray::DoubleCapacity(){
    capacity *= 2;                            // double capacity
    int *newStack = new int[capacity];        // create newStack
    
    for (int i = 0 ; i < capacity/2; i++) {   // copy element to newStack
        newStack[i] = stack[i];
    }
    delete [] stack;              // release the memory of stack
    stack = newStack;             // redirect stack to newStack
}

void StackArray::Push(int x){
    
    if (top == capacity - 1) {    // if stack is full, double the capacity
        DoubleCapacity();
    }
    stack[++top] = x;             // update top and put x into stack
}

void StackArray::Pop(){
    
    if (Empty()) {          // if stack is empty, there is nothing to pop
        return;
    }
    top--;                    // update top
    //    stack[top] = 0;         // (*1)
    //    stack[top].~T();        // (*2)
}

bool StackArray::Empty(){
    return (top == -1);
}

int StackArray::Top(){
    if (Empty()) {     // check if stack is empty
        return -1;
    }
    return stack[top];   // return the top element
}

int StackArray::Size(){
    return top+1;        // return the number of elements in stack
}
