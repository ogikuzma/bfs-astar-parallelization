# OpenMP parallelization of BFS and A* algorithms
Motivation for this project was try to parallelize Breadth-First search and A* algoritms which represent one of the most fundamental graph traversal and path search algorithms. Path search was done on graph data structures which were created from cell matricies that have obstacles and algorithms should find the path from the first to the last cell avoiding obstacles. Parallelization was done using OpenMP API for C programming language. Parts of the algorithms that were parallelized are the ones that are in charge of adding new nodes into FIFO or priority queues. Since that is not CPU complex operation, all parallel solutions have given the slower results than sequential solutions. The only case where parallelization produceced quicker results is the case when there was a simulation of complex processing put in BFS algorithm whenever a node from FIFO queue was popped. For all the results with different starting cell matricies, please see the table below.

## Results
BFS
|             | 100x100 | 500x500 | 1000x1000 | 20x20 with overload |
| ----------- | ------- | ------- | --------- | ------------------- |
| sequential  |  0.01s  |  0.58s  |  4.87s    |        32.14s       |
| parallel    |  0.02s  |  2.06s  |  7.64s    |        7.72s        |

A*
|             | 100x100 | 500x500 | 1000x1000 |
| ----------- | ------- | ------- | --------- |
| sequential  |  0.21s  |  7.87s  |  38.81s   |
| parallel    |  0.32s  |  10.46s |  50.05s   |
