#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

bool compare(pair<int, int> a, pair<int, int> b)
{
    if (a.second == b.second) return a.first < b.first;
    return a.second < b.second;
}

struct Candidate_route
{
    vector<int> route;
    int length;
    int backpack_length;
    double crowding_degree;
};

class NSGA2
{
public:
    NSGA2(vector<vector<int>> graph, vector<int> objects, int cache_size, int max_round) :Graph(graph), object_size(objects), backpack_capacity(cache_size), max_round(max_round) {}
    vector<vector<int>> Graph;
    vector<int> object_size;
    void init();
    void crossover(int);
    void mutation(int);
    void non_domination_sort(int);
    void crowding_distance(vector<vector<int>>, int);
    void update_candidate(vector<vector<int>>, int);
    int route_length(vector<int>);//the first goal function
    int route_length_in_backpack(vector<int>);//the second goal function

    default_random_engine generator;
    vector<Candidate_route> candidate;
    const int backpack_capacity;
    int candidate_num;
    int varient;
    int node_num;
    int max_round;
};
void NSGA2::init()
{
    generator = default_random_engine(time(NULL));
    node_num = Graph.size();
    candidate_num = ceil(sqrt(node_num));
    varient = ceil(sqrt(node_num));
    for (int i = 0; i < candidate_num; i++)
    {
        Candidate_route random_route;
        for (int j = 0; j < node_num; j++)
            random_route.route.push_back(j);
        random_shuffle(random_route.route.begin(), random_route.route.end());
        random_route.length = route_length(random_route.route);
        random_route.backpack_length = route_length_in_backpack(random_route.route);
        candidate.push_back(random_route);
    }
}
int NSGA2::route_length(vector<int> route)
{
    int length = 0;
    for (int i = 1; i < route.size(); i++)
        length += Graph[route[i - 1]][route[i]];
    return length;
}
int NSGA2::route_length_in_backpack(vector<int> route)
{
    int total_length = 0;
    int current_capacity = object_size[route[0]];
    for (int i = 1; i < route.size(); i++)
    {
        if (current_capacity + object_size[route[i]] > backpack_capacity)
            current_capacity = object_size[route[i]];
        else
        {
            current_capacity += object_size[route[i]];
            total_length += Graph[route[i - 1]][route[i]];
        }
    }
    return total_length;
}


void NSGA2::crossover(int round)
{
    uniform_int_distribution<int> distribution_int(0, node_num - varient);
    auto rand_int = bind(distribution_int, generator);

    for (int i = 0; i < candidate_num; i++)
    {
        for (int j = i + 1; j < candidate_num; j++)
        {
            vector<int>parent_A = candidate[i].route;
            vector<int>parent_B = candidate[j].route;
            vector<int>children = parent_B;
            int subroute_start = rand_int();
            int insert_position = rand_int();
            for (int node = 0; node < varient; node++)
            {
                int num = parent_A[subroute_start + node];
                for (int k = 0; k < children.size(); k++)
                    if (children[k] == num)
                    {
                        children.erase(children.begin() + k);
                        break;
                    }
            }
            for (int node = varient - 1; node >= 0; node--)
                children.insert(children.begin() + insert_position, parent_A[subroute_start + node]);
            bool same = 0;
            for (Candidate_route routes : candidate)
            {
                if (children == routes.route)
                {
                    same = 1;
                    break;
                }
            }
            if (same)continue;
            candidate.push_back({ children,route_length(children),route_length_in_backpack(children),0 });
        }
    }
    mutation(round);
}
void NSGA2::mutation(int round)
{
    uniform_int_distribution<int> distribution_int(0, node_num - 1);
    auto rand_int = bind(distribution_int, generator);

    int base_num = candidate.size();
    for (int i = 0; i < base_num; i++)
    {
        vector<int> base = candidate[i].route;

        int rand1 = rand_int(); int rand2 = rand_int();
        while (rand2 == rand1)rand2 = rand_int();

        swap(base[rand1], base[rand2]);

        bool same = 0;
        for (Candidate_route routes : candidate)
        {
            if (base == routes.route)
            {
                same = 1;
                break;
            }
        }
        if (same)continue;
        candidate.push_back({ base,route_length(base),route_length_in_backpack(base),0 });
    }
    non_domination_sort(round);
}

void NSGA2::non_domination_sort(int round)
{
    unordered_map<int, int> dominated_num;
    unordered_map<int, vector<int>> dominating_set;
    for (int i = 0; i < candidate.size(); i++)dominated_num[i] = 0;
    for (int i = 0; i < candidate.size(); i++)// find dominating relation
        for (int j = i + 1; j < candidate.size(); j++)
        {
            if (candidate[i].length >= candidate[j].length && candidate[i].backpack_length >= candidate[j].backpack_length)//i dominate j
            {
                if (candidate[i].length > candidate[j].length || candidate[i].backpack_length > candidate[j].backpack_length)
                {
                    dominated_num[j] += 1;
                    dominating_set[i].push_back(j);
                }
            }
            else if (candidate[i].length <= candidate[j].length && candidate[i].backpack_length <= candidate[j].backpack_length)//j dominate i
            {
                if (candidate[i].length < candidate[j].length || candidate[i].backpack_length < candidate[j].backpack_length)
                {
                    dominated_num[i] += 1;
                    dominating_set[j].push_back(i);
                }
            }
            // else no dominate
        }
    vector<vector<int>> pareto_rank;
    while (1)//find each Pareto rank
    {
        vector<int> layer;
        vector<int> post_decrease;
        for (auto& iter : dominated_num)
        {
            if (iter.second == 0)//non-dominated solution for current level
            {
                layer.push_back(iter.first);
                for (int num : dominating_set[iter.first])
                {
                    post_decrease.push_back(num);
                }
                iter.second = -1;
            }
        }
        if (layer.empty())break;
        pareto_rank.push_back(layer);
        for (int num : post_decrease)
            dominated_num[num]--;
    }
    crowding_distance(pareto_rank, round);
}

void NSGA2::crowding_distance(vector<vector<int>> pareto_rank, int round)
{
    vector<int> crowding(candidate.size(), 0);
    for (int i = 0; i < candidate.size(); i++)candidate[i].crowding_degree = 0;
    for (vector<int> layer : pareto_rank)
    {
        //crowding distance of first goal function
        vector<pair<int, int>> first_goal_value;
        for (int num : layer) first_goal_value.push_back(pair<int, int>(num, candidate[num].length));
        sort(first_goal_value.begin(), first_goal_value.end(), compare);
        candidate[first_goal_value[0].first].crowding_degree = INF;// head and tale have infinite crowding distance
        candidate[first_goal_value[layer.size() - 1].first].crowding_degree = INF;
        double normalize_first = double(first_goal_value[layer.size() - 1].second - first_goal_value[0].second);
        for (int i = 1; i < layer.size() - 1; i++)
        {
            candidate[first_goal_value[i].first].crowding_degree += double(first_goal_value[i + 1].second - first_goal_value[i - 1].second) / normalize_first;
        }
        //coding it directly since I have only two goal function.
        //crowding distance of second goal function
        vector<pair<int, int>> second_goal_value;
        for (int num : layer) second_goal_value.push_back(pair<int, int>(num, candidate[num].backpack_length));
        sort(second_goal_value.begin(), second_goal_value.end(), compare);
        candidate[second_goal_value[0].first].crowding_degree = INF;
        candidate[second_goal_value[layer.size() - 1].first].crowding_degree = INF;
        double normalize_second = double(second_goal_value[layer.size() - 1].second - second_goal_value[0].second);
        for (int i = 1; i < layer.size(); i++)
        {
            candidate[second_goal_value[i].first].crowding_degree += double(second_goal_value[i + 1].second - second_goal_value[i - 1].second) / normalize_second;
        }
    }
    update_candidate(pareto_rank, round);
}

void NSGA2::update_candidate(vector<vector<int>>pareto_rank, int round)
{
    vector<Candidate_route> new_candidate;
    for (vector<int> layer : pareto_rank)
    {
        if (new_candidate.size() + layer.size() <= candidate_num)
            for (int num : layer)
                new_candidate.push_back(candidate[num]);
        else
        {
            vector<pair<double, int>> crowdings;
            for (int num : layer)
                crowdings.push_back(pair<double, int>(candidate[num].crowding_degree, num));
            sort(crowdings.begin(), crowdings.end());
            for (int i = crowdings.size() - 1; new_candidate.size() < candidate_num; i--)
                new_candidate.push_back(candidate[crowdings[i].second]);
            break;
        }
    }
    candidate = new_candidate;

    Candidate_route route = candidate[0];
    cout << "-------------\n";
    for (int num : route.route)cout << num << " ";
    cout << endl;
    cout << route.length << "  " << route.backpack_length << endl;
    cout << "-------------\n";

    if (round > max_round)return;
    crossover(round + 1);
}

int main()
{
    const int node_num = 25;
    const int capacity = 20;

    default_random_engine generator = default_random_engine(time(NULL));
    uniform_int_distribution<int> distribution_int(1, 2 * node_num - 1);
    uniform_int_distribution<int> random_object_size(1, 5);
    auto rand_int = bind(distribution_int, generator);
    auto rand_size = bind(random_object_size, generator);

    vector<vector<int>> graph(node_num, vector<int>(node_num));
    vector<int> object(node_num, 0);
    for (int i = 0; i < node_num; i++)
    {
        for (int j = 0; j < node_num; j++)
        {
            if (i == j) graph[i][j] = 0;
            else if (i > j)graph[i][j] = graph[j][i];
            else graph[i][j] = rand_int();
        }
    }
    for (int i = 0; i < node_num; i++)
        object[i] = rand_size();

    for (int i = 0; i < node_num; i++)
    {
        for (int j = 0; j < node_num; j++)
        {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
    for (int num : object) cout << num << " ";
    cout << endl;
    NSGA2 solver(graph, object, capacity, 10);
    solver.init();
    solver.crossover(1);
}
/*
    for(Candidate_route route:candidate)
    {
        cout<<"-------------\n";
        for(int num:route.route)cout<<num<<" ";
        cout<<endl;
        cout<<route.length<<"  "<<route.backpack_length<<endl;
        cout<<"-------------\n";
    }
*/
