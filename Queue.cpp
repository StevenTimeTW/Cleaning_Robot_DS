//
//  Queue.cpp
//  Robot
//
//  Created by Steven on 2019/11/15.
//  Copyright © 2019 Steven. All rights reserved.
//

#include "Queue.hpp"

#include <iostream>
void QueueArraySequential::DoubleCapacity(){
    capacity *= 2;
    int *newQueue = new int[capacity];
    int j = -1;
    for (int i = front+1; i <= back; i++) {
        j++;
        newQueue[j] = queue[i];
    }
    front = -1;       // 新的array從0開始, 把舊的array"整段平移", front跟back要更新
    back = j;
    delete [] queue;
    queue = newQueue;
}
void QueueArraySequential::Push(int x){
    
    if (Full()) {
        DoubleCapacity();
    }
    queue[++back] = x;
}
void QueueArraySequential::Pop(){
    
    if (Empty()) {
        return;
    }
    front++;
}
bool QueueArraySequential::Full(){
    
    return (back + 1 == capacity);
}
bool QueueArraySequential::Empty(){
    
    return (front  == back);
}
int QueueArraySequential::Front(){
    
    if (Empty()) {
        return -1;
    }
    
    return queue[front+1];
}
int QueueArraySequential::Back(){
    
    if (Empty()) {
        return -1;
    }
    
    return queue[back];
}
int QueueArraySequential::Size(){
    
    return (back - front);
}
