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

// STL
#include <sstream>

// Custom
#include "GraphOpening.h"

Graph CreateDemoGraph();

int main(int argc, char *argv[])
{
  Graph g = CreateDemoGraph();
  WriteGraph(g, "original.dot");
  
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
