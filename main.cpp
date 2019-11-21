//
//  main.cpp
//  Cleaning_Robot
//
//  Created by Steven on 2019/11/13.
//  Copyright © 2019 Steven. All rights reserved.
//

#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <string>

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
    
    // 設做 Private Members 才能讀取 Private Members
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

class Cleaning_Robot{
    
private:
    int *Color,*Distance,*Predecessor;   // Index 表示各個 Vertex，Color 表是否經過（ 1 為經過、0 為尚未經過 ）
    int Count;
    stack<int> S; // Sort the index by order，越上面距離越遠。
    vector<int> Print_Step_Array;
    
public:
    Cleaning_Robot():Count(0){};
    Matrix_To_Adjacency_Matrix Steven;
    void BFS();
    void Clean();
    void Print();
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
    
    queue<int> Q;
    int Visiting = Steven.Charging_Location_Index;
    // 只找與起點有連接的 Vertex
    if ( Color[Visiting] == 0){
        Color[Visiting] = 1;
        Distance[Visiting] = 0;
        Predecessor[Visiting] = -1;
        Q.push(Visiting);
        S.push(Visiting);
        
        while(!Q.empty()){
            Visiting = Q.front();
            Q.pop();
            for (int i =0; i<Total_Elements;i++){
                if(Steven.Adjacency_Matrix[Visiting][i]==1 && Color[i]==0){
                    Q.push(i);
                    S.push(i);
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
    while(!S.empty()){
        if(Color[S.top()]!=0){
            S.pop();
        }
        
        else{
            int Visiting = S.top();
            Color[Visiting] = 1;
            stack<int> temp;
            stack<int> temp_2;
            // 從最遠處開始往原點走（作為走去的路線。）
            while(Visiting!=-1){
                temp.push(Visiting);
                Color[Visiting] = 1;
                Visiting = Predecessor[Visiting];
            }
            
            while(!temp.empty()){
                Print_Step_Array.push_back(temp.top());
                temp_2.push(temp.top());
                temp.pop();
            }
            
            temp_2.pop();
            while(temp_2.top()!=Steven.Charging_Location_Index){
                Print_Step_Array.push_back(temp_2.top());    // 將路徑儲存在 Vector 裡。
                temp_2.pop();
            }
            S.pop();
        }
    }
    Print_Step_Array.push_back(Steven.Charging_Location_Index); // 最後一步要走回來原點。
    
    Print();
}

void Cleaning_Robot::Print(){
    
    // 把初始點在充電位置的移動數目 -1。
    cout << Print_Step_Array.size()-1 << endl;
    
    // 將原先 Index 轉為行、列的表達法。
    for (int i=0;i<Print_Step_Array.size();i++){
        cout <<Print_Step_Array[i]/Steven.Columns_Num <<" " <<Print_Step_Array[i]%Steven.Columns_Num <<endl;
    }
    
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
