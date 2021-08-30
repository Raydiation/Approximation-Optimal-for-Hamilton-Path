# NSGA2
It's a multiobjective optimization algorithms base on GA.  

I add a constrain :  
Suppose that when we pass a node, we will recieve a object with size s_n (depend on node) into my backpack.
And each backpack has same capacity. Notice that we the order of objects in backpack is same as the order we pick it up.

And my goal is that (can be more):
* Total route length is as large as possible.
* In each backpack, the connetivity of nodes(depend on the edge we choose) where the objects belong to as high as possible.

The core of my NSGA2 algorithm is that:

* Crossover: For each pair (parent_A,parent_B) of candidates, pick a continuous segment from parent_A (length decided by the value of "varient") and then pick a randomly insert to parent_B to make a new candidate. (The original same nodes in parent_B should be remove.)
* Mutation: For each candidate, randomly pick two nodes and swap their position to make a new candidate.
* Non-dominating sort: We put these candidate on xy-plane(since I have only two goal function), repeat that: 
  1. Set rank=0
  2. Find the non-dominated points and set them pareto_rank=rank
  3. Delete the points with pareto_rank=rank, rank=rank+1, repeat 1.
  4. Until there is no points.
* Calculate crowding distance :
  * For each layer : 
      1. For each goal function: find f_max and f_min. Sort the point by the function value,set crowding[0]=crowding[last]=inf,  
          and set crowding_degree[i] = crowding_degree[i]+(f(i+1)-f(i-1))/(f_max-f_min)
* Update candidate list: Pick new candidate by small layer number first, otherwise pick by large crowding number.
