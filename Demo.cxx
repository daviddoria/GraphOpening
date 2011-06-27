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
#include "GraphOpening.h"

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 2)
    {
    std::cerr << "Required arguments: input.dot" << std::endl;
    return -1;
    }
    
  // Parse arguments
  std::string inputFileName = argv[1];
  
  // Output arguments
  std::cout << "Input: " << inputFileName << std::endl;
  
  Graph g = ReadGraph(inputFileName);
  
  std::cout << "Original graph: " << std::endl;
  OutputEdges(g);
  
  // Initialize the eroded graph to the original graph
  Graph erodedGraph = g;
  
  unsigned int numberOfIterations = 2;
  for(unsigned int i = 0; i < numberOfIterations; ++i)
    {
    std::cout << std::endl << "Erosion " << i << std::endl;
  
    erodedGraph = Erode(erodedGraph);
  
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
    dilatedGraph = Dilate(dilatedGraph, g);
  
    std::stringstream ss;
    ss << "dilated_" << i << ".dot";
    Graph invisibleEdgeGraph = CreateInvisibleEdgeGraph(g, dilatedGraph);
    WriteGraphWithVisibility(invisibleEdgeGraph, ss.str());
    }
  
  return EXIT_SUCCESS;
}
