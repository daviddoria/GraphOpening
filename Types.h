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

#ifndef TYPES_H
#define TYPES_H

// STL
#include <string>

// Boost
#include <boost/graph/adjacency_list.hpp>

// We need a bool to store visibility of each edge
struct EdgeVisibility
{
  // This should be used throughout the code, then the VisibilityString should be set based on this right before writing.
  bool visible;
  
  // Because of the way the dynamic property must be set to write the graph, this will need to be used.
  // However, we use the above bool to track the edge visibility during the execution, and at the very end
  // we set the string based on the bool.
  std::string VisibilityString; 
  
  
};

typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, EdgeVisibility> Graph;

#endif
