# Approximation-Optimal-for-Hamilton-Path  

The code is used to find the "maximal" Hamilton Path on a non-negative weight graph.   

### 0815  
Currently, only Brute Force and SA. (Simulated annealing)  

Althought Dynamic Programming is a best strategy for the small case, but it need a lot of space and more difficult to implement, I didn't do it.   
#### Brute Force  
Used for small case. (# of node<10?)  
#### SA  
Randomly swap two node to find a good solution. But the method of scoring a route can be reinforced.  

### 0817  
Add Ant colony optimization algorithms(ACO). Although the result is on a good way but it's a trouble for me to adjust the parameters.

#### ACO  
Need to find a good way to denote those parameter. (alpha, beta, Q, initial pheromone).

### 0818
Add GA algorithm, crossover and mutation can be reinforced.  

#### GA  
The core of my GA algorithm is 3 steps:  
1. Crossover: For each pair (parent_A,parent_B) of candidates, pick a continuous segment from parent_A (length decided by the value of "varient") and then pick a randomly insert to parent_B to make a new candidate. (The original same nodes in parent_B should be remove.)
2. Mutation: For each candidate, randomly pick two nodes and swap their position to make a new candidate.
3. Update candidate list: Pick (value of "candidate_num") candidates with highest score as next round candidates.

-------------
Current problem : 
Suppose that when we pass a node, we will recieve a object with size s_n (depend on node) into my backpack.  
And each backpack has same capacity.  
And my goal is that (pick one or both two) :
* Use less backpack as possible.  
* In each backpack, the connetivity of nodes(depend on the edge we choose) where the objects belong to as high as possible.
