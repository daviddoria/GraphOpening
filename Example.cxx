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

// This program shows a typical usage. It reads a graph from a file, performs
// the morphological opening on it, and then writes the result to a file.

// STL
#include <fstream>
#include <iostream>
#include <string>

// Boost
#include <boost/graph/graphviz.hpp>

// Custom
#include "GraphOpening.h"

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 3)
    {
    std::cerr << "Required arguments: input.dot output.dot" << std::endl;
    return -1;
    }
  
  // Parse arguments
  std::string inputFileName = argv[1];
  std::string outputFileName = argv[2];
  
  // Read the graph
  Graph graph = ReadGraph(inputFileName);

  Graph openedGraph = OpenGraph(graph, 2);
  
  WriteGraph(openedGraph, outputFileName);
  
  return EXIT_SUCCESS;
}
