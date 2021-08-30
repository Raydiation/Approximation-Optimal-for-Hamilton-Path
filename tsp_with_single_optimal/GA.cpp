#include<bits/stdc++.h>
using namespace std;

bool compare(pair<vector<int>,int> a,pair<vector<int>,int> b){return a.second>b.second;}

class GA
{
public:
    GA(vector<vector<int>> graph,int max_round):Graph(graph),node_num(graph.size()),max_round(max_round){}
    void Init();
    int route_length(vector<int>);
    void crossover(int);
    void mutation(int);
    void update_candidate(int);

    default_random_engine generator;
    vector<vector<int>> Graph;
    vector<pair<vector<int>,int>> candidate_route;
    int candidate_num;
    int varient;
    int node_num;
    int max_round;
};
int GA::route_length(vector<int> route)
{
    int length=0;
    for(int i=1;i<route.size();i++)
        length+=Graph[route[i-1]][route[i]];
    return length;
}
void GA::Init()
{
    generator = default_random_engine(time(NULL));
    candidate_num=ceil(sqrt(node_num));
    varient=candidate_num;
    for(int i=0;i<candidate_num;i++)
    {
        vector<int> random_route;
        for(int j=0;j<node_num;j++)
            random_route.push_back(j);
        random_shuffle(random_route.begin(),random_route.end());
        candidate_route.push_back(pair<vector<int>,int>(random_route,route_length(random_route)));
    }
}
void GA::crossover(int round)
{
    uniform_int_distribution<int> distribution_int(0,node_num-varient);
    auto rand_int=bind(distribution_int,generator);

    for(int i=0;i<candidate_num;i++)
    {
        for(int j=i+1;j<candidate_num;j++)
        {
            vector<int>parent_A=candidate_route[i].first;
            vector<int>parent_B=candidate_route[j].first;
            vector<int>children=parent_B;
            int subroute_start=rand_int();
            int insert_position=rand_int();
            for(int node=0;node<varient;node++)
            {
                int num=parent_A[subroute_start+node];
                for(int k=0;k<children.size();k++)
                    if(children[k]==num)
                    {
                        children.erase(children.begin()+k);
                        break;
                    }
            }
            for(int node=varient-1;node>=0;node--)
                children.insert(children.begin()+insert_position,parent_A[subroute_start+node]);
            candidate_route.push_back(pair<vector<int>,int>(children,route_length(children)));
        }
    }
    mutation(round);
}
void GA::mutation(int round)
{
    default_random_engine generator = default_random_engine(time(NULL));
    uniform_int_distribution<int> distribution_int(0,node_num-1);
    auto rand_int=bind(distribution_int,generator);

    int base_num=candidate_route.size();
    for(int i=0;i<base_num;i++)
    {
        vector<int> base=candidate_route[i].first;

        int rand1=rand_int();int rand2=rand_int();
        while(rand2==rand1)rand2=rand_int();

        swap(base[rand1],base[rand2]);

        candidate_route.push_back(pair<vector<int>,int>(base,route_length(base)));
    }
    update_candidate(round+1);
}
void GA::update_candidate(int round)
{
    if(round)
    {
        sort(candidate_route.begin(),candidate_route.end(),compare);
        vector<pair<vector<int>,int>> new_candidate;
        int index=0;
        while(new_candidate.size()<=candidate_num)
        {
            bool flag=0;
            for(int i=0;i<new_candidate.size();i++)
                if(candidate_route[index].first==new_candidate[i].first)
                    flag=1;
            if(!flag)
                new_candidate.push_back(candidate_route[index]);
            index++;
        }
        candidate_route=new_candidate;
        for(int num:candidate_route[0].first)
            cout<<num<<" ";
        cout<<candidate_route[0].second<<endl;
    }
    if(round>max_round)return;
    crossover(round);
}
