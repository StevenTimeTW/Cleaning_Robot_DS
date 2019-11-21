//
//  Queue.hpp
//  Robot
//
//  Created by Steven on 2019/11/15.
//  Copyright © 2019 Steven. All rights reserved.
//

#ifndef Queue_hpp
#define Queue_hpp

#include <stdio.h>


class QueueArraySequential{
private:
    int capacity, front, back;
    int *queue;
    void DoubleCapacity();
public:
    QueueArraySequential():capacity(1000),front(-1),back(-1){
        queue = new int[capacity];
    };
    void Push(int x);
    void Pop();
    bool Empty();
    bool Full();
    int Front();
    int Back();
    int Size();
};

#endif /* Queue_hpp */
