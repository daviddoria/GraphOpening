cmake_minimum_required(VERSION 2.6)

PROJECT(GraphOpening)

#Boost
FIND_PACKAGE(Boost)

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS})

#### Executables ####
ADD_EXECUTABLE(GraphOpeningTrackingExample GraphOpeningTrackingExample.cxx Helpers.cxx GraphOpeningTracking.cxx)
target_link_libraries(GraphOpeningTrackingExample boost_graph)

ADD_EXECUTABLE(GraphOpeningNaiveExample GraphOpeningNaiveExample.cxx Helpers.cxx GraphOpeningNaive.cxx)
target_link_libraries(GraphOpeningNaiveExample boost_graph)

ADD_EXECUTABLE(Demo Demo.cxx Helpers.cxx GraphOpeningNaive.cxx)
target_link_libraries(Demo boost_graph)

# ADD_EXECUTABLE(CreateDemoGraph CreateDemoGraph.cxx GraphOpening.cxx)
# target_link_libraries(CreateDemoGraph boost_graph)
