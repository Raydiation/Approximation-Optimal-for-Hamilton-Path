#include<bits/stdc++.h>
using namespace std;

class SA
{
public:
    SA(vector<vector<int>>graph,int max_round):Graph(graph),Max_round(max_round),node_num(graph.size()){}
    double Score(vector<int>);
    vector<int> Rand_start();
    void main_algorithm(int);

    vector<vector<int>> Graph;
    int Max_round;
    int node_num;

    vector<int> goodOrder;
    int goodOrderScore;
};

vector<int> SA::Rand_start()
{
    vector<int> temp;
    for(int i=0;i<node_num;i++)temp.push_back(i);
    random_shuffle(temp.begin(),temp.end());
    return temp;
}
double SA::Score(vector<int> order)
{
    double score = 0;
    for(int i=1;i<order.size();i++)
        score=score+Graph[order[i-1]][order[i]];
    return score;
}
void SA::main_algorithm(int Max_Round)
{
    random_device rand_seed;
    default_random_engine generator = default_random_engine(rand_seed());
    uniform_int_distribution<int> distribution_int(0,node_num-1);
    uniform_real_distribution<double> distribution_real(0,1);
    auto rand_int=bind(distribution_int,generator);
    auto rand_real=bind(distribution_real,generator);

    vector<int> current_order = Rand_start();
    vector<int> next_order=current_order;
    double current_score = Score(current_order);
    double next_score;

    goodOrder=current_order;
    goodOrderScore=current_score;

    int round=0;
    while(round<Max_Round)
    {
        int rand1=rand_int();int rand2=rand_int();
        while(rand2==rand1)rand2=rand_int();

        next_order=current_order;
        swap(next_order[rand1],next_order[rand2]);
        next_score=Score(next_order);

        if(next_score/current_score>rand_real())
        {
            current_order=next_order;
            current_score=next_score;
            if(goodOrderScore<current_score)
            {
                goodOrder=current_order;
                goodOrderScore=current_score;
            }
        }
        round++;
        if(!(round%100))
        {
            for(int num:goodOrder)
                cout<<num<<" ";
            cout<<endl;
            cout<<"Score : "<<goodOrderScore<<endl;
        }
    }
    return;
}


