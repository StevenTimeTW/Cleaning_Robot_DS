
//  此版本使用 Graph 的 Adjacency Matrix 進行 BFS，作為前進時的最短路徑。
//  利用 Stack 將距離由近至遠依序存下，作為下次出發的點。
//  回程時優先判斷四周 一、是否有未造訪過得、距離較大的點 二、是否有未造訪過、距離較小的點，
//  若都已造訪過，則走訪 Predecessor 直至原點。

//  Cleaning_Robot
//  ----------------------
//  Created by Steven on 2019/11/13.
//  Copyright © 2019 Steven. All rights reserved.
//

#include <iostream>
#include <string>
#include "Stack.hpp"
#include "Queue.hpp"
using namespace std;

class Cleaning_Robot;
// 此 Class 用來讀入 Input 的資訊以及將 Map 轉換成 Adjacency Matrix
class Matrix_To_Adjacency_Matrix{
private:
    int Rows_Num, Columns_Num;
    char *Input_Matrix;
    int **Adjacency_Matrix;
    int Charging_Location_Index;
    int Battery_Life;
public:
    friend class Cleaning_Robot;
    // Constructor
    Matrix_To_Adjacency_Matrix():
    Rows_Num(0),Columns_Num(0),Battery_Life(0),
    Input_Matrix(nullptr),Adjacency_Matrix(nullptr),
    Charging_Location_Index(0){};
    
    Matrix_To_Adjacency_Matrix(int Rows,int Columns,char *Map_Matrix,int Battery):
    Rows_Num(Rows),Columns_Num(Columns),Battery_Life(Battery),
    Input_Matrix(Map_Matrix),
    Charging_Location_Index(0){
        Adjacency_Matrix = new int *[Rows*Columns];
        for (int i = 0; i<Rows*Columns; i++){
            Adjacency_Matrix[i]=new int [Rows*Columns]{0};
        }
        Matrix_Transfer();
    };
    void Matrix_Transfer();
    void Print_Adjacency_Matrix();
};
// 將每個 Vertex 向下、向右連 Edge，就可以求出所有 Edge
void Matrix_To_Adjacency_Matrix::Matrix_Transfer(){
    for(int i=0; i<Rows_Num-1; i++){
        for(int j=0; j<Columns_Num-1;j++){
            // 與下一列判斷是否有 Edge，直到 Row Number -1 行
            if((Input_Matrix[j+i*Columns_Num]=='0'||Input_Matrix[j+i*Columns_Num]=='R')&&
               (Input_Matrix[j+(i+1)*Columns_Num]=='0'|| Input_Matrix[j+(i+1)*Columns_Num]=='R')){
                Adjacency_Matrix[j+i*Columns_Num][j+(i+1)*Columns_Num]=1;
                Adjacency_Matrix[j+(i+1)*Columns_Num][j+i*Columns_Num]=1;
            }
            // 每一個元素與右側判斷是否有 Edge，直到 Column Number - 1 行
            if((Input_Matrix[j+i*Columns_Num]=='0'||Input_Matrix[j+i*Columns_Num]=='R')&&
               (Input_Matrix[j+1+i*Columns_Num]=='0'|| Input_Matrix[j+1+i*Columns_Num]=='R')){
                Adjacency_Matrix[j+i*Columns_Num][j+1+i*Columns_Num]=1;
                Adjacency_Matrix[j+1+i*Columns_Num][j+i*Columns_Num]=1;
            }
            if(Input_Matrix[j+i*Columns_Num]=='R') Charging_Location_Index=j+i*Columns_Num;
        }
    }
    // 若 R 在最底層的話，位置不會被找到，故多加此迴圈。
    for(int k=0; k<Columns_Num-1;k++){
        if(Input_Matrix[k+(Rows_Num-1)*Columns_Num]=='R') Charging_Location_Index=k+(Rows_Num-1)*Columns_Num;
    }
}
// Not necessary : To check whether the adjacency_Matrix builded or not
void Matrix_To_Adjacency_Matrix::Print_Adjacency_Matrix(){
    for(int i=0;i<Rows_Num*Columns_Num;i++){
        for(int j=0;j<Rows_Num*Columns_Num;j++){
            cout << Adjacency_Matrix[i][j];
        }
        cout <<endl;
    }
}
class Cleaning_Robot{
private:
    int *Color,*Distance,*Predecessor;   // Index 表示各個 Vertex，Color 表是否經過（ 1 為經過、0 為尚未經過 ）
    StackArray S; // Sort the index by order in Stack，越上層距離越遠。
    QueueArraySequential Print_Step_Array;
public:
    Cleaning_Robot(){};
    
    ~Cleaning_Robot(){};
    Matrix_To_Adjacency_Matrix Steven;
    void BFS();
    void Clean();
    void Print();
    void Going_Back(int Visiting_Location,int Battery_Left);
};
void Cleaning_Robot::BFS(){
    int Total_Elements =Steven.Rows_Num*Steven.Columns_Num;
    Color = new int[Total_Elements];
    Distance = new int[Total_Elements];
    Predecessor = new int[Total_Elements];
    
    for (int i = 0; i < Total_Elements; i++) {  // 初始化
        Color[i] = 0;                       // 將所有 Vertex 設為尚未經過
        Predecessor[i] = -1;                // -1 表示沒有predecessor
        Distance[i] = Total_Elements+1;         // 將所有 Distance 設為不可能初始化為不可能走到的距離
    }
    QueueArraySequential Q;
    int Visiting = Steven.Charging_Location_Index;
    // 只找與起點有連接的 Vertex
    if ( Color[Visiting] == 0){
        Color[Visiting] = 1;
        Distance[Visiting] = 0;
        Predecessor[Visiting] = -1;
        Q.Push(Visiting);
        S.Push(Visiting);
        
        while(!Q.Empty()){
            Visiting = Q.Front();
            Q.Pop();
            for (int i =0; i<Total_Elements;i++){
                if(Steven.Adjacency_Matrix[Visiting][i]==1 && Color[i]==0){
                    Q.Push(i);
                    S.Push(i);
                    Color[i] = 1;
                    Distance[i]=Distance[Visiting]+1;
                    Predecessor[i]=Visiting;
                }
            }
            Color[Visiting]=1;
        }
    }
    // 將顏色全改為未經過。
    for (int i=0;i<Total_Elements;i++){
        Color[i]=0;
    }
}
void Cleaning_Robot::Clean(){
    
    // 從距離最遠的開始且未造訪過的作為出發點。
    while(!S.Empty()){
        
        // 當最遠處被造訪過，及 pop 掉，繼續 while 迴圈
        if(Color[S.Top()]!=0){
            S.Pop();
        }
        
        // 依最短路徑走到 S.Top()
        else{
            int Visiting = S.Top();
            int Battery_Left = Steven.Battery_Life;
            int Visiting_Back = S.Top();
            
            Color[Visiting] = 1;
            StackArray temp;
            // 從最遠處開始往原點走（作為走去的路線。）
            while(Visiting!=-1){
                temp.Push(Visiting);
                Color[Visiting] = 1;
                Visiting = Predecessor[Visiting];
                Battery_Left --;
            }
            Battery_Left ++; // 從原點起算經過 n 格，走了 n - 1 步
            
            while(!temp.Empty()){
                Print_Step_Array.Push(temp.Top());
                temp.Pop();
            }
            Going_Back(Visiting_Back,Battery_Left);
            S.Pop();
        }
    }
    Print_Step_Array.Push(Steven.Charging_Location_Index);
    Print();
}
void Cleaning_Robot::Print(){
    // 把初始點在充電位置的移動數目 -1。
    cout << Print_Step_Array.Size()-1 << endl;
    // 將原先 Index 轉為行、列的表達法。
    for (int i=0;i<Print_Step_Array.Size();i++){
        cout <<Print_Step_Array.Front()/Steven.Columns_Num <<" " <<Print_Step_Array.Front()%Steven.Columns_Num <<endl;
        Print_Step_Array.Pop();
    }
}
// 回程時，以未造訪過的節點優先。
void Cleaning_Robot::Going_Back(int Visiting_Location,int Battery_Left){
    
    while(Visiting_Location!=Steven.Charging_Location_Index){
        
        // 向右方確認是否距離更大，且電池壽命足夠。
        if( Color[Visiting_Location+1]==0 &&
            Steven.Adjacency_Matrix[Visiting_Location][Visiting_Location+1]==1 &&
            Battery_Left-1 >= Distance[Visiting_Location+1]&&
           Distance[Visiting_Location+1] > Distance[Visiting_Location]
           ){
            Visiting_Location = Visiting_Location +1;
            Print_Step_Array.Push(Visiting_Location);
            Color[Visiting_Location]=1;
            Battery_Left --;
        }
        
        // 向下方確認是否距離更大，且電池壽命足夠。
        else if( Color[Visiting_Location+Steven.Columns_Num]==0 &&
           Steven.Adjacency_Matrix[Visiting_Location][Visiting_Location+Steven.Columns_Num]==1 &&
           Battery_Left-1 >= Distance[Visiting_Location+Steven.Columns_Num]&&
                Distance[Visiting_Location+Steven.Columns_Num] > Distance[Visiting_Location]){
            Visiting_Location = Visiting_Location+Steven.Columns_Num;
            Print_Step_Array.Push(Visiting_Location);
            Color[Visiting_Location]=1;
            Battery_Left --;
        }
        
        // 向左方確認是否距離更大，且電池壽命足夠。
        else if( Color[Visiting_Location-1]==0 &&
           Steven.Adjacency_Matrix[Visiting_Location][Visiting_Location-1]==1 &&
           Battery_Left-1 >= Distance[Visiting_Location-1]&&
           Distance[Visiting_Location-1] > Distance[Visiting_Location]
                ){
            
            Visiting_Location = Visiting_Location -1;
            Print_Step_Array.Push(Visiting_Location);
            Color[Visiting_Location]=1;

            Battery_Left --;
        }
        
        // 向上方確認是否距離更大，且電池壽命足夠。
        else if( Color[Visiting_Location-Steven.Columns_Num]==0 &&
                Steven.Adjacency_Matrix[Visiting_Location][Visiting_Location-Steven.Columns_Num]==1 &&
                Battery_Left-1 >= Distance[Visiting_Location-Steven.Columns_Num]&&
                   Distance[Visiting_Location-Steven.Columns_Num] > Distance[Visiting_Location]){
            
            Visiting_Location = Visiting_Location-Steven.Columns_Num;
            Print_Step_Array.Push(Visiting_Location);
            Color[Visiting_Location]=1;

            Battery_Left --;
        }
        // 向右方確認是否距離更小，且電池壽命足夠。
        else if( Color[Visiting_Location+1]==0 &&
           Steven.Adjacency_Matrix[Visiting_Location][Visiting_Location+1]==1 &&
           Battery_Left-1 >= Distance[Visiting_Location+1]){
            Visiting_Location = Visiting_Location +1;
            Print_Step_Array.Push(Visiting_Location);
            Color[Visiting_Location]=1;
            Battery_Left --;
        }
        // 向下方確認是否距離更小，且電池壽命足夠。
        else if( Color[Visiting_Location+Steven.Columns_Num]==0 &&
                Steven.Adjacency_Matrix[Visiting_Location][Visiting_Location+Steven.Columns_Num]==1 &&
                Battery_Left-1 >= Distance[Visiting_Location+Steven.Columns_Num]){
            Visiting_Location = Visiting_Location+Steven.Columns_Num;
            Print_Step_Array.Push(Visiting_Location);
            Color[Visiting_Location]=1;
            Battery_Left --;
        }
        // 向左方確認是否距離更小，且電池壽命足夠。
        else if( Color[Visiting_Location-1]==0 &&
                Steven.Adjacency_Matrix[Visiting_Location][Visiting_Location-1]==1 &&
                Battery_Left-1 >= Distance[Visiting_Location-1]){
            
            Visiting_Location = Visiting_Location -1;
            Print_Step_Array.Push(Visiting_Location);
            Color[Visiting_Location]=1;
            
            Battery_Left --;
        }
        // 向上方確認是否距離更小，且電池壽命足夠。
        else if( Color[Visiting_Location-Steven.Columns_Num]==0 &&
                Steven.Adjacency_Matrix[Visiting_Location][Visiting_Location-Steven.Columns_Num]==1 &&
                Battery_Left-1 >= Distance[Visiting_Location-Steven.Columns_Num]){
            
            Visiting_Location = Visiting_Location-Steven.Columns_Num;
            Print_Step_Array.Push(Visiting_Location);
            Color[Visiting_Location]=1;
            
            Battery_Left --;
        }
        // 若此 Vertex 四周都沒尚未造訪的點，則走回 Predecessor 一格。
        else {
            Visiting_Location = Predecessor[Visiting_Location];
            
            if (Visiting_Location==Steven.Charging_Location_Index) break;
            Print_Step_Array.Push(Visiting_Location);
            Color[Visiting_Location]=1;
            Battery_Left --;
        }
    }
    return;
    
}
int main() {
    int Rows, Columns,Battery_Life;
    string Elements_By_Rows;
    cin >> Rows >> Columns >> Battery_Life ;
    char *Map_Matrix = new char [Rows*Columns];
    // Input the Map Matrix
    for(int i=0; i<Rows; i++){
        cin >> Elements_By_Rows ;
        for(int j=0; j<Columns; j++){
            Map_Matrix[j+i*Columns] = Elements_By_Rows[j];
        }
    }
    // Convert it to the Adjacency Matrix
    Cleaning_Robot A;
    Matrix_To_Adjacency_Matrix Temp_Matrix (Rows,Columns,Map_Matrix,Battery_Life);
    A.Steven =Temp_Matrix;
    A.BFS();
    A.Clean();
    return 0;
}
