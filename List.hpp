//
//  List.hpp
//  Matrix_To_List
//
//  Created by Steven on 2019/11/15.
//  Copyright © 2019 Steven. All rights reserved.
//

#ifndef List_hpp
#define List_hpp

#include <stdio.h>
using namespace std;

class LinkedList;    // 為了將class LinkedList設成class ListNode的friend,
// 需要先宣告
class ListNode{
public:

    int data;
    ListNode *next;
    
    ListNode():data(0),next(0){};
    ListNode(int a):data(a),next(0){};
    friend class LinkedList;
};

class LinkedList{
public:
    int size;                // size是用來記錄Linked list的長度, 非必要
    ListNode *first;
    // list的第一個node
    LinkedList():first(0),size(0){};
    void Push_front(int x);     // 在list的開頭新增node
    void Push_back(int x);      // 在list的尾巴新增node
    bool Find (int x);
};

#endif /* List_hpp */
