#include<bits/stdc++.h>
using namespace std;

class Brute_Force
{
public:
    Brute_Force(vector<vector<int>> graph):Graph(graph),node_num(graph.size()),goodOrderScore(0){}
    void Force_solution();
    void permutation(int[],int,int);

    vector<vector<int>> Graph;
    int node_num;

    vector<int> goodOrder;
    int goodOrderScore;
};

void Brute_Force::Force_solution()
{
    int arr[node_num];
    for(int i=0;i<node_num;i++) arr[i]=i;
    permutation(arr,0,node_num);
}
void Brute_Force::permutation(int arr[],int current,int _size)
{
    if(current==_size)
    {
        vector<int> order(1,arr[0]);
        int orderScore=0;
        for(int i=1;i<_size;i++)
        {
            order.push_back(arr[i]);
            orderScore+=Graph[arr[i-1]][arr[i]];
        }
        if(orderScore>goodOrderScore)
        {
            goodOrderScore=orderScore;
            goodOrder=order;
        }
    }
    else
    {
        for(int i=current;i<_size;i++)
        {
            swap(arr[i],arr[current]);
            permutation(arr,current+1,_size);
            swap(arr[i],arr[current]);
        }
    }
    return;
}
