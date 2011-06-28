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

#include "GraphOpeningTracking.h"
#include "Helpers.h"

Graph ErodeTracking(const Graph& g, const std::vector<Graph::vertex_descriptor>& inputPotentialEndPoints,
                                    std::vector<Graph::vertex_descriptor>& outputPotentialEndPoints)
{
  /*
  Remove all edges attached to an EndPoint
  */
  Graph eroded = g;
  std::cout << "g has " << boost::num_vertices(g) << " and eroded has " << boost::num_vertices(eroded) << std::endl;
  
  outputPotentialEndPoints.clear();
  
  std::cout << "There are " << inputPotentialEndPoints.size() << " end points." << std::endl;

  // Remove edges containing an end point
  for(unsigned int i = 0; i < inputPotentialEndPoints.size(); ++i)
    {
    if(!IsEndPoint(g, inputPotentialEndPoints[i]))
      {
      continue;
      }
    // Get the other vertex attached to the end point
    std::vector<Graph::vertex_descriptor> neighbors = GetNeighbors(g, inputPotentialEndPoints[i]);

    std::cout << "Removing edge between: " << neighbors[0] << " and " << inputPotentialEndPoints[i] << std::endl;
  
    boost::remove_edge(neighbors[0],inputPotentialEndPoints[i],eroded);
    //boost::remove_vertex<>(endPoints[i],eroded); // do not remove the vertex or the name/id of the vertices will change
    
    outputPotentialEndPoints.push_back(neighbors[0]);
    }

  //std::cout << "eroded has " << boost::num_vertices(eroded) << std::endl;
  return eroded;
}

Graph DilateTracking(const Graph& g, const Graph& parent, 
                     const std::vector<Graph::vertex_descriptor>& inputPotentialEndPoints,
                     std::vector<Graph::vertex_descriptor>& outputPotentialEndPoints)
{
  /*
 Add back an edge to every end point
 */
  
  Graph dilated = g;

  outputPotentialEndPoints.clear();
  
  // Add back edges that were removed
  for(unsigned int i = 0; i < inputPotentialEndPoints.size(); ++i)
    {
    if(!IsEndPoint(g, inputPotentialEndPoints[i]))
      {
      continue;
      }
    // Get attached vertices in parent graph
    std::vector<Graph::vertex_descriptor> neighbors = GetNeighbors(parent, inputPotentialEndPoints[i]);
    
    for(unsigned int neighbor = 0; neighbor < neighbors.size(); neighbor++)
      {
      // Check to make sure this edge doesn't already exist (i.e that it is the one that is causing this to be an end point)
      if(EdgeExists(dilated, neighbors[neighbor], inputPotentialEndPoints[i]))
	{
	//std::cout << "Edge between: " << neighbors[neighbor] << " and " << endPoints[i] << " already exists!" << std::endl;
	continue;
	}
      boost::add_edge(neighbors[neighbor], inputPotentialEndPoints[i], dilated);
      std::cout << "Adding edge between: " << neighbors[neighbor] << " and " << inputPotentialEndPoints[i] << std::endl;
      outputPotentialEndPoints.push_back(neighbors[neighbor]);
      } // end loop over neighbors
  
    } // end loop over endpoints

  //std::cout << "dilated has " << boost::num_vertices(dilated) << std::endl;
  return dilated;
}


Graph OpenGraphFixedTracking(const Graph& g, unsigned int numberOfIterations)
{
  // Initialize the eroded graph to the original graph
  Graph erodedGraph = g;
  
  std::vector<Graph::vertex_descriptor> inputPotentialEndPoints = FindEndPoints(g);
  std::vector<Graph::vertex_descriptor> outputPotentialEndPoints;
  
  for(unsigned int i = 0; i < numberOfIterations; ++i)
    {
    std::cout << std::endl << "TrackingErosion " << i << std::endl;
  
    erodedGraph = ErodeTracking(erodedGraph, inputPotentialEndPoints, outputPotentialEndPoints);
    inputPotentialEndPoints = outputPotentialEndPoints;
    }
    
  // Initialize the dilated graph to the last eroded graph
  Graph dilatedGraph = erodedGraph;
  
  for(unsigned int i = 0; i < numberOfIterations; ++i)
    {
    std::cout << std::endl << "TrackingDilation " << i << std::endl;
    dilatedGraph = DilateTracking(dilatedGraph, g, inputPotentialEndPoints, outputPotentialEndPoints);
    inputPotentialEndPoints = outputPotentialEndPoints;
    }
    
  return dilatedGraph;
}
