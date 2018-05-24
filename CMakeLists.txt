cmake_minimum_required(VERSION 3.1)
project(signals)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -std=c++11")

set(SOURCE_FILES candlestick.cpp onlinetvwap.cpp barrier.cpp)
add_library(signals SHARED ${SOURCE_FILES})
