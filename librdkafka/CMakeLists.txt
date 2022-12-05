cmake_minimum_required(VERSION 3.10)

set(CMAKE_VERBOSE_MAKEFILE OFF)

# specify the C++ standard 
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-O2 -g)

set(THIRD_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../../third_lib)

project(demo_rapidjson VERSION 1.0)

set(BIN_NAME_P "kafka_producer")
set(BIN_NAME_C "kafka_consumer")


include_directories(${THIRD_LIB}/include)
include_directories(${THIRD_LIB}/librdkafka/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)


aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/producer SRC_P)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/consumer SRC_C)


# add the executable 
add_executable(${BIN_NAME_P} ${SRC_P})
add_executable(${BIN_NAME_C} ${SRC_C})

# 该指令的作用为将目标文件与库文件进行链接 
# target_link_libraries官方文档:https://cmake.org/cmake/help/latest/command/target_link_libraries.html 
#set(THIRD_LIB "kafka_producer")
#
# LIBS要放在最后
set(LIBS "-lm ${THIRD_LIB}/lib/libzstd.a -lsasl2 -Wl,-Bstatic -lssl -Wl,-Bdynamic -Wl,-Bstatic -lcrypto -Wl,-Bdynamic -Wl,-Bstatic -lz -Wl,-Bdynamic -ldl -lpthread -lrt -lstdc++")
target_link_libraries(${BIN_NAME_P}
    ${THIRD_LIB}/librdkafka/lib/librdkafka++.a ${THIRD_LIB}/librdkafka/lib/librdkafka.a
    ${LIBS}
)
target_link_libraries(${BIN_NAME_C}
    ${THIRD_LIB}/librdkafka/lib/librdkafka++.a ${THIRD_LIB}/librdkafka/lib/librdkafka.a
    ${LIBS}
)

# target_include_directories(CalculateSqrt PUBLIC 
#                            "${PROJECT_BINARY_DIR}" 
#                            "${PROJECT_SOURCE_DIR}/MathFunctions" 
#                            ) 