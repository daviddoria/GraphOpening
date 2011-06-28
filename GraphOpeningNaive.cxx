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

// Custom
#include "GraphOpeningNaive.h"
#include "Helpers.h"
#include "Types.h"

Graph ErodeNaive(const Graph& g)
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

Graph DilateNaive(const Graph& g, const Graph& parent)
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


Graph OpenGraphFixedNaive(const Graph& g, unsigned int numberOfIterations)
{
 
  // Initialize the eroded graph to the original graph
  Graph erodedGraph = g;
  
  
  for(unsigned int i = 0; i < numberOfIterations; ++i)
    {
    std::cout << std::endl << "NaiveErosion " << i << std::endl;
  
    erodedGraph = ErodeNaive(erodedGraph);
    }
    
  // Initialize the dilated graph to the last eroded graph
  Graph dilatedGraph = erodedGraph;
  
  for(unsigned int i = 0; i < numberOfIterations; ++i)
    {
    std::cout << std::endl << "NaiveDilation " << i << std::endl;
    dilatedGraph = DilateNaive(dilatedGraph, g);
    }
    
  return dilatedGraph;
}

