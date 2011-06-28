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

// Custom
#include "Helpers.h"

// STL
#include <fstream>

// Boost
#include <boost/graph/graphviz.hpp> // For writing graphs to a file

unsigned int CountNeighbors(const Graph& g, const Graph::vertex_descriptor& v)
{
  return boost::degree(v, g);
}

std::vector<Graph::vertex_descriptor> GetNeighbors(const Graph& g, const Graph::vertex_descriptor& v)
{
  std::pair<Graph::adjacency_iterator, Graph::adjacency_iterator> neighborsMap = boost::adjacent_vertices(v, g);

  std::vector<Graph::vertex_descriptor> neighbors;
  for(; neighborsMap.first != neighborsMap.second; ++neighborsMap.first)
    {
    neighbors.push_back(*neighborsMap.first);
    }
  return neighbors;
}

bool IsEndPoint(const Graph& g, const Graph::vertex_descriptor& v)
{
  unsigned int numberOfNeighbors = CountNeighbors(g, v);
  if(numberOfNeighbors == 1)
    {
    return true;
    }
  return false;
}

std::vector<Graph::vertex_descriptor> FindEndPoints(const Graph& g)
{
  std::vector<Graph::vertex_descriptor> endPoints;
  
  for(unsigned int i = 0; i < boost::num_vertices(g); ++i)
    {
    if(IsEndPoint(g, i))
      {
      endPoints.push_back(i);
      }
    }
  return endPoints;
}


bool EdgeExists(const Graph& g, const Graph::vertex_descriptor& v0, const Graph::vertex_descriptor& v1)
{
  return boost::edge(v0, v1, g).second;
}

void WriteGraph(const Graph& g, const std::string& fileName)
{
  std::ofstream fout(fileName.c_str());
  boost::write_graphviz(fout,g); 
}

void WriteGraphWithVisibility(const Graph& inputGraph, const std::string& fileName)
{
  Graph g = inputGraph;
  unsigned int numberOfInvisibleEdges = 0;
  
  // Iterate over all of the edges and set the edges string property from their bool property
  std::pair<Graph::edge_iterator, Graph::edge_iterator> edgeIteratorRange = boost::edges(g);
  for(Graph::edge_iterator edgeIterator = edgeIteratorRange.first; edgeIterator != edgeIteratorRange.second; ++edgeIterator)
    {
    if(g[*edgeIterator].visible)
      {
      g[*edgeIterator].VisibilityString = "normal";
      }
    else
      {
      g[*edgeIterator].VisibilityString = "invis";
      numberOfInvisibleEdges++;
      }
    }
  
  // Setup the dynamic property which will be used to write the graph with edge properties
  boost::dynamic_properties dp;
  dp.property("style", get(&EdgeVisibility::VisibilityString, g));
  dp.property("node_id", get(boost::vertex_index, g));

  // Write out the graph
  //boost::write_graphviz_dp(std::cout, g, dp);
  std::ofstream fout(fileName.c_str());
  boost::write_graphviz_dp(fout, g, dp);
}

Graph ReadGraph(const std::string& fileName)
{
  // Create a graph type with a vertex property to store the id of the vertices in the graphviz file
  typedef boost::property < boost::vertex_name_t, std::string> VertexProperty;
  typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS, VertexProperty> GraphFromFile;
  
  // Read the graphviz file
  GraphFromFile graphFromFile;
  boost::dynamic_properties dp;
  
  boost::property_map<GraphFromFile, boost::vertex_name_t>::type name =
    get(boost::vertex_name, graphFromFile);
  dp.property("node_id",name);
  
  std::ifstream fin(fileName.c_str());
  boost::read_graphviz(fin, graphFromFile, dp);

  // Create a property_map of the input vertex ids
  boost::property_map<GraphFromFile, boost::vertex_name_t>::type value = boost::get(boost::vertex_name_t(), graphFromFile);
  
  // Create a new graph of the desired type
  Graph graph(boost::num_vertices(graphFromFile));
  
  // Iterate over the edges of the input graph and create edges in the output graph on the corresponding vertices
  std::pair<GraphFromFile::edge_iterator, GraphFromFile::edge_iterator> edgePair;
  for(edgePair = boost::edges(graphFromFile); edgePair.first != edgePair.second; ++edgePair.first)
  {
    std::stringstream ssSource(value[boost::source(*edgePair.first, graphFromFile)]);
    std::stringstream ssTarget(value[boost::target(*edgePair.first, graphFromFile)]);
    unsigned int source = 0;
    unsigned int target = 0;
    ssSource >> source;
    ssTarget >> target;
    boost::add_edge(source, target, graph);
  }
  
  // Set all the edges to visible
  std::pair<Graph::edge_iterator, Graph::edge_iterator> edgeRange = boost::edges(graph);
  for(Graph::edge_iterator iterator = edgeRange.first; iterator != edgeRange.second; ++iterator)
    {
    graph[*iterator].visible = true;
    }
  return graph;
}


void OutputEdges(const Graph& g)
{
  std::cout << std::endl << "OutputEdges()" << std::endl;
  std::pair<Graph::edge_iterator, Graph::edge_iterator> edgeIteratorRange = boost::edges(g);
  for(Graph::edge_iterator edgeIterator = edgeIteratorRange.first; edgeIterator != edgeIteratorRange.second; ++edgeIterator)
    {
      std::cout << "Edge exists between " << boost::source(*edgeIterator, g) << " and " 
                <<  boost::target(*edgeIterator, g) << std::endl;
    }  
}

Graph CreateInvisibleEdgeGraph(const Graph& fullGraph, const Graph& currentGraph)
{
  std::cout << std::endl << "CreateInvisibleEdgeGraph()" << std::endl;
  Graph invisibleEdgeGraph = fullGraph;
  
  // Iterate over all of the edges of the original graph. If an edge doesn't exist in the currentGraph, mark it as invisible
  // in the output graph
  std::pair<Graph::edge_iterator, Graph::edge_iterator> edgeIteratorRange = boost::edges(invisibleEdgeGraph);
  for(Graph::edge_iterator edgeIterator = edgeIteratorRange.first; edgeIterator != edgeIteratorRange.second; ++edgeIterator)
    {
    if(EdgeExists(currentGraph, boost::target(*edgeIterator, fullGraph), boost::source(*edgeIterator, fullGraph)))
      {
      invisibleEdgeGraph[*edgeIterator].visible = true;
      }
    else
      {
      invisibleEdgeGraph[*edgeIterator].visible = false;
      }
    }  

  return invisibleEdgeGraph;
}

unsigned int CountInvisibleEdges(const Graph g)
{
  unsigned int numberOfInvisibleEdges = 0;
  // Iterate over all of the edges of the original graph.
  std::pair<Graph::edge_iterator, Graph::edge_iterator> edgeIteratorRange = boost::edges(g);
  for(Graph::edge_iterator edgeIterator = edgeIteratorRange.first; edgeIterator != edgeIteratorRange.second; ++edgeIterator)
    {
    std::cout << g[*edgeIterator].visible << " ";
    if(!g[*edgeIterator].visible)
      {
      numberOfInvisibleEdges++;
      }
    }
  std::cout << std::endl;
  return numberOfInvisibleEdges;
}

void OutputEdgeVisibility(const Graph g)
{
  // Iterate over all of the edges of the original graph.
  std::pair<Graph::edge_iterator, Graph::edge_iterator> edgeIteratorRange = boost::edges(g);
  for(Graph::edge_iterator edgeIterator = edgeIteratorRange.first; edgeIterator != edgeIteratorRange.second; ++edgeIterator)
    {
    std::cout << g[*edgeIterator].visible << " ";
    }
  std::cout << std::endl;
}
