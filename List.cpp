//
//  List.cpp
//  Matrix_To_List
//
//  Created by Steven on 2019/11/15.
//  Copyright © 2019 Steven. All rights reserved.
//

#include "List.hpp"
#include <fstream>

using namespace std;

void LinkedList::Push_front(int x){
    
    ListNode *newNode = new ListNode(x);   // 配置新的記憶體
    newNode->next = first;                 // 先把first接在newNode後面
    first = newNode;                       // 再把first指向newNode所指向的記憶體位置
    size++;
}

void LinkedList::Push_back(int x){
    ListNode *newNode = new ListNode(x);   // 配置新的記憶體
    if (first == 0) {                      // 若list沒有node, 令newNode為first
        first = newNode;
        return;
    }
    ListNode *current = first;
    while (current->next != 0) {           // Traversal
        current = current->next;
    }
    current->next = newNode; // 將newNode接在list的尾巴
    size++;
}

bool LinkedList::Find(int x){
    
    ListNode *current = first;
    
    while (current!=0){
        
        if(current->data ==x ) return 1;
        current = current->next;
    }
    
    return 0;
}

