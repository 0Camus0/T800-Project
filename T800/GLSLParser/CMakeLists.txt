cmake_minimum_required (VERSION 2.6)
project (GLSLParser)

set(CMAKE_CXX_STANDARD 11)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../Lib)
file(GLOB SOURCES *.cpp)

add_library(GLSLParser ${SOURCES})
target_include_directories(GLSLParser PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/Include)

install(TARGETS GLSLParser DESTINATION Lib)