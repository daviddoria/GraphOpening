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

#ifndef GRAPHOPENINGTRACKING_H
#define GRAPHOPENINGTRACKING_H

#include "Types.h"

// Perform a morphological dilation on a graph
Graph DilateTracking(const Graph& g, const Graph& parent,
                     const std::vector<Graph::vertex_descriptor>& inputPotentialEndPoints,
                     std::vector<Graph::vertex_descriptor>& outputPotentialEndPoints);

// Perform a morphological erosion on a graph
Graph ErodeTracking(const Graph& g, const std::vector<Graph::vertex_descriptor>& inputPotentialEndPoints,
                                    std::vector<Graph::vertex_descriptor>& outputPotentialEndPoints);


// This function performs the morphological opening on the graph 'g' a fixed number (numberOfIterations)
// of times and returns the resulting graph with edges removed. The end points are tracked after each
// erosion and dilation operation so that an exhaustive search is only necessary at the beginning.
Graph OpenGraphFixedTracking(const Graph& g, unsigned int numberOfIterations);

#endif
