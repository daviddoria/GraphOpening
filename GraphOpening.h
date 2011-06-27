/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

// STL
#include <iostream>

// Boost
#include <boost/graph/adjacency_list.hpp>


//typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

// We need a name property to use the graphviz graph reader
typedef boost::property < boost::vertex_name_t, std::string> VertexProperty;
// We need a bool to store visibility of each edge
struct EdgeVisibility
{
  bool visible;
  
  // Because of the way the dynamic property must be set to write the graph, this will need to be used.
  // However, we use the above bool to track the edge visibility during the execution, and at the very end
  // we set the string based on the bool.
  std::string VisibilityString; 
  
  
};
typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS, VertexProperty, EdgeVisibility> Graph;
  
// Get a list of the vertices in the graph which are end points.
std::vector<Graph::vertex_descriptor> FindEndPoints(Graph);

// Perform a morphological dilation on a graph
Graph Dilate(Graph g, Graph parent);

// Perform a morphological erosion on a graph
Graph Erode(Graph);

// Determine how many neighbors a vertex has
unsigned int CountNeighbors(Graph, Graph::vertex_descriptor);

// Get a list of the vertex_descriptor's of all of a vertexes neighbors.
std::vector<Graph::vertex_descriptor> GetNeighbors(Graph, Graph::vertex_descriptor);

// Determine if the specified vertex is an endpoint. That is, does it have exactly 1 neighbor?
bool IsEndPoint(Graph, Graph::vertex_descriptor);

// Determine of an edge between 'v0' and 'v1' exists in 'g'
bool EdgeExists(Graph g, Graph::vertex_descriptor v0, Graph::vertex_descriptor v1);

// Write a graph to a file, with no invisible edges
void WriteGraph(Graph& g, const std::string& fileName);

// Write a graph to a file, possible with invisible edges
void WriteGraphWithVisibility(Graph& g, const std::string& fileName);

// Read a graph from a .dot file.
Graph ReadGraph(const std::string& fileName);

// Output all of the elements of a vector, space delimited.
template <typename T>
void OutputVector(std::vector<T>& v)
{
  for(unsigned int i = 0; i < v.size(); ++i)
    {
    std::cout << v[i] << " ";
    }
  std::cout << std::endl;
}

// This function performs the morphological opening on the graph 'g' and returns the resulting
// graph with edges removed
Graph OpenGraph(const Graph& g, unsigned int numberOfIterations);

// This function takes the original 'fullGrap'h (with all of the edges) and the the 'currentGraph'
// which has missing edges and creates a graph to return which has all of the edges, but missing
// edges in 'currentGraph' are marked as invisible.
Graph CreateInvisibleEdgeGraph(const Graph& fullGraph, const Graph& currentGraph);

// Output the edges of a graph
void OutputEdges(const Graph& g);