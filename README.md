# graphs
Graph library for learning purpose



# Project Description
Create a C++ template library that defines a graph data structure and implements several common graph algorithms. The library should be designed to be reusable and easily integrated into other projects.

# features
class template for graphs. Only adjacency matrix for this version.


# Requirements

X. The library should define a class template for a graph that can store the vertices and edges. The class should be parameterized by the type of the vertex and edge data.

X. The library should include member functions for adding and removing vertices and edges, and for checking if a particular edge exists. The member functions should be defined in the header file.

The library should implement the following algorithms as template functions:

X Breadth-first search (BFS)
X Depth-first search (DFS)
Dijkstra's shortest path algorithm
Prim's minimum spanning tree algorithm
The library should be designed to work with any type of vertex and edge data, as long as the data types provide the necessary operators and functions for the graph algorithms to work. The library should use template metaprogramming to check that the required operators and functions are defined for the vertex and edge data types.

The library should be well-documented, with comments explaining the purpose of each function and block of code. The documentation should include instructions on how to use the library in other projects.

The library should include a test suite that tests the functionality of the graph class and the graph algorithms.

The library should be packaged as a header-only library, so that it can be easily integrated into other projects.
