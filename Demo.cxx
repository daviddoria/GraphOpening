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

// This program duplicates the functionality of the OpenGraph function
// in GraphOpening.cxx . It does this so that the graph can be written
// at each step for explanatory purposes.

// STL
#include <sstream>

// Custom
#include "GraphOpeningNaive.h"
#include "Helpers.h"

Graph CreateGraph();

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 2)
    {
    std::cerr << "Required arguments: numberOfIterations" << std::endl;
    return -1;
    }
    
  unsigned int numberOfIterations = 0;
  std::stringstream ss(argv[1]);
  ss >> numberOfIterations;
  
  // Output arguments
  std::cout << "Number of iterations: " << numberOfIterations << std::endl;
  
  Graph g = CreateGraph();
  WriteGraph(g, "original.dot");
  
  std::cout << "Original graph: " << std::endl;
  OutputEdges(g);
  
  // Initialize the eroded graph to the original graph
  Graph erodedGraph = g;
  
  for(unsigned int i = 0; i < numberOfIterations; ++i)
    {
    std::cout << std::endl << "Erosion " << i << std::endl;
  
    erodedGraph = ErodeNaive(erodedGraph);
  
    std::stringstream ss;
    ss << "eroded_" << i << ".dot";
    //WriteGraphWithVisibility(erodedGraph, ss.str());
    Graph invisibleEdgeGraph = CreateInvisibleEdgeGraph(g, erodedGraph);
    
    WriteGraphWithVisibility(invisibleEdgeGraph, ss.str());

    }
    
  // Initialize the dilated graph to the last eroded graph
  Graph dilatedGraph = erodedGraph;
  
  for(unsigned int i = 0; i < numberOfIterations; ++i)
    {
    std::cout << std::endl << "Dilation " << i << std::endl;
    dilatedGraph = DilateNaive(dilatedGraph, g);
  
    std::stringstream ss;
    ss << "dilated_" << i << ".dot";
    Graph invisibleEdgeGraph = CreateInvisibleEdgeGraph(g, dilatedGraph);
    WriteGraphWithVisibility(invisibleEdgeGraph, ss.str());
    }
  
  return EXIT_SUCCESS;
}

Graph CreateGraph()
{
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
