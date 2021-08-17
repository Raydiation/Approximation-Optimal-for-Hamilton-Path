#include<bits/stdc++.h>
using namespace std;
class ACO
{
public:
    ACO(vector<vector<int>> graph,int round_max):Graph(graph),node_num(graph.size()),round_max(round_max){}
    double greedy();
    void init();
    void Round(int);
    void Compute_Transintion_Probability();
    void update_pheromone(int,vector<vector<int>>);

    vector<int>goodPath;
    vector<vector<int>>Graph;
    vector<vector<double>>pheromone;
    vector<vector<double>>transition_probability;
    double pheromone_decrease;
    int goodPathLength;
    int num_of_ants;
    int node_num;
    int round_max;
    double alpha;
    double beta;
    double Q;
};
void ACO::init()
{
    double initial_pheromone=greedy();
    pheromone = vector<vector<double>>(node_num,vector<double>(node_num,initial_pheromone));
    transition_probability = vector<vector<double>>(node_num,vector<double>(node_num));
    pheromone_decrease=0.8;
    goodPathLength=0;
    num_of_ants=node_num;
    alpha=1;
    beta=2;
    Q=1000;
}
double ACO::greedy()
{
    int visited[node_num];
    memset(visited,0,sizeof(visited));

    int num_of_walk = 0;
    int total_length=0;
    int current_position = 0;
    int visited_node = 1;

    visited[0]=1;
    while(visited_node<node_num)
    {
        int max_position=current_position;
        for(int i=0;i<node_num;i++)
        {
            if(visited[i])continue;
            if(Graph[current_position][i]>Graph[current_position][max_position])
                max_position=i;
        }
        total_length+=Graph[current_position][max_position];
        visited[max_position]=1;
        visited_node++;
        current_position=max_position;
    }
    for(int i=0;i<node_num;i++)
        for(int j=i+1;j<node_num;j++)
            if(Graph[i][j])num_of_walk++;
    return 1.0/double(total_length*num_of_walk);
}
void ACO::Compute_Transintion_Probability()
{
    for(int i=0;i<node_num;i++)
        for(int j=0;j<node_num;j++)
            if(Graph[i][j])
                transition_probability[i][j]=pow(pheromone[i][j],alpha)*pow(Graph[i][j],beta);
    return;
}

void ACO::Round(int round)
{
    default_random_engine generator = default_random_engine(time(NULL));
    uniform_int_distribution<int> distribution_int(0,node_num-1);
    uniform_real_distribution<double> distribution_real(0,1);
    auto rand_int=bind(distribution_int,generator);
    auto rand_real=bind(distribution_real,generator);
    Compute_Transintion_Probability();
    vector<vector<int>> path(node_num);
    for(int ant=0;ant<num_of_ants;ant++)
    {
        vector<pair<int,double>>visit(node_num);
        int current_position=rand_int();
        path[ant].clear();

        for(int i=0;i<node_num;i++)visit[i].first=i;
        visit.erase(visit.begin()+current_position);
        path[ant].push_back(current_position);

        for(int vis_num=1;vis_num<node_num;vis_num++)//construct TSP route
        {
            double dice = rand_real();
            int next_node;

            visit[0].second=transition_probability[current_position][visit[0].first];
            for(int i=1;i<visit.size();i++)
            {
                visit[i].second=transition_probability[current_position][visit[i].first];
                visit[i].second+=visit[i-1].second;//accumulate
            }
            for(int i=0;i<visit.size();i++)
                visit[i].second/=visit[visit.size()-1].second;

            for(int i=visit.size()-1;i>=0;i--)
            {
                if(dice>visit[i].second)
                {
                    next_node=visit[i+1].first;
                    visit.erase(visit.begin()+(i+1));
                    path[ant].push_back(next_node);
                    current_position=next_node;
                    break;
                }
                if(!i)
                {
                    next_node=visit[0].first;
                    visit.erase(visit.begin()+0);
                    path[ant].push_back(next_node);
                    current_position=next_node;
                    break;
                }
            }
        }
    }
    update_pheromone(round,path);
}
void ACO::update_pheromone(int round,vector<vector<int>> path)
{
    for(int i=0;i<node_num;i++)
        for(int j=0;j<node_num;j++)
            pheromone[i][j]*=(1.0-pheromone_decrease);
    for(int i=0;i<path.size();i++)
    {
        double total_length=0;
        for(int j=1;j<path[i].size();j++)
            total_length+=Graph[path[i][j-1]][path[i][j]];
        for(int j=1;j<path[i].size();j++)
        {
            pheromone[path[i][j-1]][path[i][j]]+=(Q/total_length);
            pheromone[path[i][j]][path[i][j-1]]+=(Q/total_length);
        }
        if(total_length>goodPathLength)
        {
            goodPathLength=total_length;
            goodPath=path[i];
        }
    }
    cout<<"Path Length : "<<goodPathLength<<endl;
    for(int i=0;i<goodPath.size();i++)
        cout<<goodPath[i]<<" ";
    cout<<endl;
    if(round<round_max)Round(round+1);
}
