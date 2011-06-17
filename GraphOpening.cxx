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
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

std::vector<Graph::vertex_descriptor> FindEndPoints(Graph);

Graph Dilate(Graph g, Graph parent);

Graph Erode(Graph);

unsigned int CountNeighbors(Graph, Graph::vertex_descriptor);

std::vector<Graph::vertex_descriptor> GetNeighbors(Graph, Graph::vertex_descriptor);

bool IsEndPoint(Graph, Graph::vertex_descriptor);

bool EdgeExists(Graph g, Graph::vertex_descriptor v0, Graph::vertex_descriptor v1);

Graph CreateDemoGraph();

template <typename T>
void OutputVector(std::vector<T>& v)
{
  for(unsigned int i = 0; i < v.size(); ++i)
    {
    std::cout << v[i] << " ";
    }
  std::cout << std::endl;
}

int main(int argc, char *argv[])
{
  Graph g = CreateDemoGraph();
  
  // Initialize the eroded graph to the original graph
  Graph erodedGraph = g;
  
  unsigned int numberOfIterations = 2;
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
  
  return EXIT_SUCCESS;
}

Graph CreateDemoGraph()
{
    // Create the following graph
  /*
   *                                    v8          v14
   *                                    |           |
   * v0 --v1--v2--v3--v4---v5---v6--v7--v11--v12--v13--v15--v16--v17--v18
   *                                    |
   *                                    v9
   *                                    |
   *                                    v10
   *
   * After 1 erosion, the graph should look like:
   *
   *
   * v1--v2--v3--v4--v5--v6--v7--v11--v12--v13--v15--v16--v17
   *                              |
   *                              v9
   *
   * After 2 erosions, the graph should look like:
   * 
   * v2--v3--v4--v5--v6--v7--v11--v12--v13--v15---v16
   *
   * After the first dilation, the graph should look like:
   * 
   * v1--v2--v3--v4--v5--v6--v7--v11--v12--v13--v15--v16--v17
   *
   * After the second dilation, the graph should look like:
   *
   * v0--v1--v2--v3--v4--v5--v6--v7--v11--v12--v13--v15--v16--v17--v18
   */

  Graph g(19);

  boost::add_edge(0,1,g);
  boost::add_edge(1,2,g);
  boost::add_edge(2,3,g);
  boost::add_edge(3,4,g);
  boost::add_edge(4,5,g);
  boost::add_edge(5,6,g);
  boost::add_edge(6,7,g);
  boost::add_edge(7,8,g);
  boost::add_edge(7,9,g);
  boost::add_edge(9,10,g);
  boost::add_edge(7,11,g);
  boost::add_edge(11,12,g);
  boost::add_edge(12,13,g);
  boost::add_edge(13,14,g);
  boost::add_edge(13,15,g);
  boost::add_edge(15,16,g);
  boost::add_edge(16,17,g);
  boost::add_edge(17,18,g);
  
  return g;
}

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