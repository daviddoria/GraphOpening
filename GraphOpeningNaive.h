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

#ifndef GRAPHOPENINGNAIVE_H
#define GRAPHOPENINGNAIVE_H

// STL
#include <iostream>

// Boost
#include <boost/graph/adjacency_list.hpp>

// Custom
#include "Types.h"

// This function performs the morphological opening on the graph 'g' a fixed number (numberOfIterations)
// of times and returns the resulting graph with edges removed. A naive method of finding the end points
// at every iteration of all erosion and dilation operations is implemented.
Graph OpenGraphFixedNaive(const Graph& g, unsigned int numberOfIterations);

// Perform a morphological dilation on a graph
Graph DilateNaive(const Graph& g, const Graph& parent);

// Perform a morphological erosion on a graph
Graph ErodeNaive(const Graph&);

#endif
