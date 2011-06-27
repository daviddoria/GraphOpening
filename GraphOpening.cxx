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

/*
This technique attempts to remove small branches from an MST while preserving the large structure.

The idea is:

Erosion step:
Remove all edges attached to EndPoints (leaf nodes). After multiple iterations of this, small branches
will be "absorbed" into a main branch.

Dilation step:
Add back edges to current EndPoints. This is NOT simply going to grow back the same tree. If we ran the erosion enough times
to absorb a branch, the branch will not grow back. If we did not, then the branch will indeed grow back.

This algorithm is based on "Efficient Closed Contour Extraction from Range Image's Edge Points"  by Angel Sappa
*/

// STL
#include <iostream>

// Boost
#include <boost/graph/graphviz.hpp> // For writing graphs to a file

// Custom
#include "GraphOpening.h"

Graph Erode(Graph g)
{
  /*
  Remove all edges attached to an EndPoint
  */
  Graph eroded = g;
  std::cout << "g has " << boost::num_vertices(g) << " and eroded has " << boost::num_vertices(eroded) << std::endl;
  
  // Find all the end points
  std::vector<Graph::vertex_descriptor> endPoints = FindEndPoints(g);
  std::cout << "End points are: " << std::endl;
  OutputVector(endPoints);
  
  std::cout << "There are " << endPoints.size() << " end points." << std::endl;

  // Remove edges containing an end point
  for(unsigned int i = 0; i < endPoints.size(); ++i)
    {
    // Get the other vertex attached to the end point
    std::vector<Graph::vertex_descriptor> neighbors = GetNeighbors(g, endPoints[i]);

    std::cout << "Removing edge between: " << neighbors[0] << " and " << endPoints[i] << std::endl;
  
    boost::remove_edge(neighbors[0],endPoints[i],eroded);
    //boost::remove_vertex<>(endPoints[i],eroded); // do not remove the vertex or the name/id of the vertices will change
    
    }

  //std::cout << "eroded has " << boost::num_vertices(eroded) << std::endl;
  return eroded;
}

Graph Dilate(Graph g, Graph parent)
{
  /*
 Add back an edge to every end point
 */
  
  Graph dilated = g;
  
  // Find all the end points
  std::vector<Graph::vertex_descriptor> endPoints = FindEndPoints(g);
  std::cout << "End points are: " << std::endl;
  OutputVector(endPoints);
    
  // Add back edges that were removed
  for(unsigned int i = 0; i < endPoints.size(); ++i)
    {
    // Get attached vertices in parent graph
    std::vector<Graph::vertex_descriptor> neighbors = GetNeighbors(parent, endPoints[i]);
    
    for(unsigned int neighbor = 0; neighbor < neighbors.size(); neighbor++)
      {
      // Check to make sure this edge doesn't already exist (i.e that it is the one that is causing this to be an end point)
      if(EdgeExists(dilated, neighbors[neighbor], endPoints[i]))
	{
	//std::cout << "Edge between: " << neighbors[neighbor] << " and " << endPoints[i] << " already exists!" << std::endl;
	continue;
	}
      boost::add_edge(neighbors[neighbor], endPoints[i], dilated);
      std::cout << "Adding edge between: " << neighbors[neighbor] << " and " << endPoints[i] << std::endl;
      } // end loop over neighbors
  
    } // end loop over endpoints

  //std::cout << "dilated has " << boost::num_vertices(dilated) << std::endl;
  return dilated;
}

unsigned int CountNeighbors(Graph g, Graph::vertex_descriptor v)
{
  std::pair<Graph::adjacency_iterator, Graph::adjacency_iterator> neighbors = boost::adjacent_vertices(v, g);

  unsigned int counter = 0;
  for(; neighbors.first != neighbors.second; ++neighbors.first)
    {
    counter++;
    //std::cout << *neighbors.first << " ";
    }
  return counter;
}

std::vector<Graph::vertex_descriptor> GetNeighbors(Graph g, Graph::vertex_descriptor v)
{
  std::pair<Graph::adjacency_iterator, Graph::adjacency_iterator> neighborsMap = boost::adjacent_vertices(v, g);

  std::vector<Graph::vertex_descriptor> neighbors;
  for(; neighborsMap.first != neighborsMap.second; ++neighborsMap.first)
    {
    neighbors.push_back(*neighborsMap.first);
    }
  return neighbors;
}

bool IsEndPoint(Graph g, Graph::vertex_descriptor v)
{
  unsigned int numberOfNeighbors = CountNeighbors(g, v);
  if(numberOfNeighbors == 1)
    {
    return true;
    }
  return false;
}

std::vector<Graph::vertex_descriptor> FindEndPoints(Graph g)
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


bool EdgeExists(Graph g, Graph::vertex_descriptor v0, Graph::vertex_descriptor v1)
{
  typedef boost::property_map<Graph, boost::vertex_index_t>::type IndexMap;
  IndexMap index = get(boost::vertex_index, g);

  typedef boost::graph_traits < Graph >::adjacency_iterator adjacency_iterator;

  std::pair<adjacency_iterator, adjacency_iterator> neighbors =
    boost::adjacent_vertices(v0, g);

  for(; neighbors.first != neighbors.second; ++neighbors.first)
    {
    //std::cout << index[*neighbors.first] << std::endl;
    if(index[*neighbors.first] == v1)
      {
      return true;
      }
    }
    
  return false;
}

void WriteGraph(Graph& g, const std::string& fileName)
{
  
  std::ofstream fout(fileName.c_str());
  boost::write_graphviz(fout,g);
  
}

void WriteGraphWithVisibility(Graph& g, const std::string& fileName)
{
  // Iterate over all of the edges, setting their string property from their bool property
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
  return graph;
}

Graph OpenGraph(const Graph& g, unsigned int numberOfIterations)
{
 
  // Initialize the eroded graph to the original graph
  Graph erodedGraph = g;
  
  
  for(unsigned int i = 0; i < numberOfIterations; ++i)
    {
    std::cout << std::endl << "Erosion " << i << std::endl;
  
    erodedGraph = Erode(erodedGraph);
    }
    
  // Initialize the dilated graph to the last eroded graph
  Graph dilatedGraph = erodedGraph;
  
  for(unsigned int i = 0; i < numberOfIterations; ++i)
    {
    std::cout << std::endl << "Dilation " << i << std::endl;
    dilatedGraph = Dilate(dilatedGraph, g);

    }
    
  return dilatedGraph;
}

void OutputEdges(const Graph& g)
{
  std::pair<Graph::edge_iterator, Graph::edge_iterator> edgeIteratorRange = boost::edges(g);
  for(Graph::edge_iterator edgeIterator = edgeIteratorRange.first; edgeIterator != edgeIteratorRange.second; ++edgeIterator)
    {
      std::cout << "Edge exists between " << boost::source(*edgeIterator, g) << " and " 
                <<  boost::target(*edgeIterator, g) << std::endl;
    }  
}

Graph CreateInvisibleEdgeGraph(const Graph& fullGraph, const Graph& currentGraph)
{
  Graph invisibleEdgeGraph = fullGraph;
  
  // Iterate over all of the edges of the original graph. If an edge doesn't exist in the currentGraph, mark it as invisible
  // in the output graph
  std::pair<Graph::edge_iterator, Graph::edge_iterator> edgeIteratorRange = boost::edges(fullGraph);
  for(Graph::edge_iterator edgeIterator = edgeIteratorRange.first; edgeIterator != edgeIteratorRange.second; ++edgeIterator)
    {
    if(EdgeExists(currentGraph, boost::target(*edgeIterator, fullGraph), boost::source(*edgeIterator, fullGraph)))
      {
      std::cout << "Edge exists between " << boost::source(*edgeIterator, fullGraph) << " and " 
                <<  boost::target(*edgeIterator, fullGraph) << std::endl;
      invisibleEdgeGraph[*edgeIterator].VisibilityString = "normal";
      }
    else
      {
      std::cout << "Edge does NOT exist between " << boost::source(*edgeIterator, fullGraph) << " and " 
		<<  boost::target(*edgeIterator, fullGraph) << std::endl;
      invisibleEdgeGraph[*edgeIterator].VisibilityString = "invis";
      }
    }  
  return invisibleEdgeGraph;
}
