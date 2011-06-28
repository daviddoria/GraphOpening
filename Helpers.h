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

#ifndef HELPERS_H
#define HELPERS_H

// Custom
#include "Types.h"

// STL
#include <vector>

// Get a list of the vertices in the graph which are end points.
std::vector<Graph::vertex_descriptor> FindEndPoints(const Graph&);

// This function takes the original 'fullGraph' (with all of the edges) and the the 'currentGraph'
// which has missing edges and creates a graph to return which has all of the edges, but missing
// edges in 'currentGraph' are marked as invisible.
Graph CreateInvisibleEdgeGraph(const Graph& fullGraph, const Graph& currentGraph);


// Determine how many neighbors a vertex has
unsigned int CountNeighbors(const Graph&, const Graph::vertex_descriptor&);

// Get a list of the vertex_descriptor's of all of a vertexes neighbors.
std::vector<Graph::vertex_descriptor> GetNeighbors(const Graph&, const Graph::vertex_descriptor&);

// Determine if the specified vertex is an endpoint. That is, does it have exactly 1 neighbor?
bool IsEndPoint(const Graph&, const Graph::vertex_descriptor&);

// Determine of an edge between 'v0' and 'v1' exists in 'g'
bool EdgeExists(const Graph& g, const Graph::vertex_descriptor& v0, const Graph::vertex_descriptor& v1);

// Write a graph to a file, with no invisible edges
void WriteGraph(const Graph& g, const std::string& fileName);

// Write a graph to a file, possible with invisible edges
void WriteGraphWithVisibility(const Graph& g, const std::string& fileName);

// Read a graph from a .dot file.
Graph ReadGraph(const std::string& fileName);

unsigned int CountInvisibleEdges(const Graph g);

void OutputEdgeVisibility(const Graph g);

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


// Output the edges of a graph
void OutputEdges(const Graph& g);

#endif
