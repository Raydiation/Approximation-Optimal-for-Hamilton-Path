#include<bits/stdc++.h>
#include"SA.cpp"
#include"Brute_Force.cpp"
using namespace std;
class TSP_solution
{
public:
    TSP_solution(vector<vector<int>> graph)
    {
        Graph = graph;
        goodOrder =vector<int>(0);
        goodOrderScore=0;
        node_num = Graph.size();
    }
    vector<int> TSP()
    {
        Brute_Force Brute_Force_solver(Graph);
        Brute_Force_solver.Force_solution();
        return Brute_Force_solver.goodOrder;
        /*int max_round=1000;
        SA SA_solver(Graph,max_round);
        SA_solver.main_algorithm(max_round);
        return SA_solver.goodOrder;*/
    }

    vector<vector<int>> Graph;
    vector<int> goodOrder;
    int goodOrderScore;
    int node_num;
};
int main()
{
    default_random_engine generator = default_random_engine(time(NULL));
    uniform_int_distribution<int> distribution_int(0,100);
    auto rand_int=bind(distribution_int,generator);

    vector<vector<int>> graph(8,vector<int>(8));
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(i==j) graph[i][j]=0;
            else if(i>j)graph[i][j]=graph[j][i];
            else graph[i][j]=rand_int();
        }
    }

    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            cout<<graph[i][j]<<" ";
        }
        cout<<endl;
    }
    TSP_solution solve(graph);
    vector<int> ans=solve.TSP();
    for(int num:ans)
        cout<<num<<" ";
}
