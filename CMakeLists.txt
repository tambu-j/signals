cmake_minimum_required(VERSION 3.1)
project(signals)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES candlestick.cpp onlinetvwap.cpp)
add_library(signals SHARED ${SOURCE_FILES})
