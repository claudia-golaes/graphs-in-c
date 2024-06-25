# Graphs in C
----------
### REQUIREMENT 1:
----------

#### Implementation of an undirected graph with adjacency lists for city locations, where each road between them has a distance.

**For requirement a):**
The implementation includes the following functions:

#### newAdjListNode
- Creates and returns a new adjacency list node with the specified destination node and cost.

#### createGraph
- Creates and returns a new graph with the specified number of nodes. Initializes the adjacency list and the array of zone names.

#### addEdge
- Adds an edge between two nodes, specifying the source and destination nodes, as well as the edge cost. Adds two nodes to the adjacency list, one for each direction.

#### DFS1
- Auxiliary function for Depth-First Search (DFS) traversal of the graph. Recursively traverses adjacent nodes of vertex `v` and marks them as part of the same island (connected component).

#### DFS2
- Traverses the graph and counts the islands (connected components) using the DFS1 function.

**For requirement b):**
Using the above functions, a new function is created to generate a new graph for each connected component and apply Kruskal's algorithm to find the minimum cost.

#### findIndex
- Searches and returns the index of a node given its name in the graph.

#### getMinimumTotalCost
- Calculates and returns the total cost of the Minimum Spanning Tree (MST) using Kruskal's algorithm. Arbitrarily selects a starting node and greedily selects edges with the smallest cost that connect a visited node to an unvisited node until an MST with V-1 edges is obtained.

#### createIslands
- Creates separate islands of the graph, calculating the minimum spanning tree cost for each island. This is achieved by constructing separate subgraphs for each island and calling the getMinimumTotalCost function for each subgraph.

#### bubbleSort
- Sorts a vector using the Bubble Sort algorithm.

----------
### REQUIREMENT 2:
----------

#### Implementation of a directed graph representing a map of an island leading to a treasure. Using Dijkstra's algorithm to calculate the most viable path for carrying the treasure: based on the smallest score (where score is the ratio of edge cost to the node depth which is the destination for the edge).

**The implementation includes the following specific functions:**

#### dfs
- This function implements Depth-First Search (DFS) to check if there exists a path between a source node and a destination node in the graph. Uses a visited vector to mark visited nodes during DFS traversal.

#### hasPath
- Takes a graph `graph`, a source node `src`, and a destination node `dest`, and returns 1 if there exists a path between the two nodes in the graph, otherwise returns 0. Uses the dfs function to check path existence.

#### printPath
- Takes a file `file`, a parent vector, a destination node `j`, a graph `graph`, and a minimum depth variable by reference. Prints the optimal path to the destination node and updates the minimum depth value.

#### dijkstra
- Takes a graph `graph`, a source node `src`, a file `file`, and a weight `weight`. Implements Dijkstra's algorithm to find the shortest path from the source node to all other nodes in the graph. Prints the minimum cost, minimum depth, and number of paths required to reach the destination.

