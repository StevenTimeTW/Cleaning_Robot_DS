//
//  Stack.hpp
//  Stack_Test
//
//  Created by Steven on 2019/11/15.
//  Copyright © 2019 Steven. All rights reserved.
//

#ifndef Stack_hpp
#define Stack_hpp

#include <stdio.h>

class StackArray{
private:
    int top;                 // index of top element
    int capacity;            // allocated memory space of array
    int *stack;              // array representing stack
    void DoubleCapacity();   // double the capacity of stack
public:
    StackArray():top(-1),capacity(1000){    // constructor
        stack = new int[capacity];       // initial state: top=-1, capacity=1
    }
    void Push(int x);
    void Pop();
    bool Empty();
    int Top();
    int Size();
};


#endif /* Stack_hpp */
