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
