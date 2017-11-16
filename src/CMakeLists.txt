cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(proto)

set(SOURCE_FILES Service.cpp Socket.cpp)

set(LIBRARIES proto)

add_library(Common ${SOURCE_FILES})
target_link_libraries(Common ${LIBRARIES})
