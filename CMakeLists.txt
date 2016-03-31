cmake_minimum_required(VERSION 3.3)
project(pa3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Cluster.cpp
    Cluster.h
    ClusteringTests.cpp
    ClusteringTests.h
    ErrorContext.cpp
    ErrorContext.h
    Exceptions.cpp
    Exceptions.h
    KMeans.cpp
    KMeans.h
    main.cpp
    point.cpp
    Point.h)

add_executable(pa3 ${SOURCE_FILES})