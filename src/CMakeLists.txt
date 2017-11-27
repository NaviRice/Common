CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
SET(CMAKE_CXX_STANDARD 14)

INCLUDE(FINDPROTOBUF)

ADD_SUBDIRECTORY(proto)

SET(SOURCE_FILES Socket.cpp Server.cpp Client.cpp Service.cpp)

SET(LIBRARIES proto)

INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})

add_executable (server Socket.cpp Server.cpp test/server_test.cpp)
add_executable (service Socket.cpp Server.cpp Service.cpp test/service_test.cpp)
add_executable (client Socket.cpp Client.cpp test/client_test.cpp)

ADD_LIBRARY(common ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(common ${LIBRARIES})

TARGET_LINK_LIBRARIES(server ${LIBRARIES})
TARGET_LINK_LIBRARIES(service ${LIBRARIES})
TARGET_LINK_LIBRARIES(client ${LIBRARIES})
