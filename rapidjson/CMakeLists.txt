cmake_minimum_required(VERSION 3.10)

# set(CMAKE_VERBOSE_MAKEFILE ON)

# specify the C++ standard 
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(THIRD_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../../third_lib)

project(demo_rapidjson VERSION 1.0)

set(BIN_NAME "demo_rapidjson")

include_directories(${THIRD_LIB}/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC)

# add the executable 
add_executable(${BIN_NAME} ${SRC})

# 该指令的作用为将目标文件与库文件进行链接 
# target_link_libraries官方文档:https://cmake.org/cmake/help/latest/command/target_link_libraries.html 
# target_link_libraries(${BIN_NAME} PUBLIC MathFunctions) 

# target_include_directories(CalculateSqrt PUBLIC 
#                            "${PROJECT_BINARY_DIR}" 
#                            "${PROJECT_SOURCE_DIR}/MathFunctions" 
#                            ) 