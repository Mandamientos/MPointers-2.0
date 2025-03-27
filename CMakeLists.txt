cmake_minimum_required(VERSION 3.16)
project(MPointers CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_PREFIX_PATH "$ENV{HOME}/grpc_install")
set(gRPC_ROOT "$ENV{HOME}/grpc_install")
set(Protobuf_ROOT "$ENV{HOME}/grpc_install")

find_package(Threads REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

set(memman_proto_srcs "${CMAKE_CURRENT_SOURCE_DIR}/gen/memman.pb.cc")
set(memman_proto_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/gen/memman.pb.h")
set(memman_grpc_srcs "${CMAKE_CURRENT_SOURCE_DIR}/gen/memman.grpc.pb.cc")
set(memman_grpc_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/gen/memman.grpc.pb.h")

add_library(memman_proto
  ${memman_grpc_srcs}
  ${memman_grpc_hdrs}
  ${memman_proto_srcs}
  ${memman_proto_hdrs}
)

target_link_libraries(memman_proto
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)

set(SERVER_SOURCES
  src/main.cpp
  src/Memory_Manager/memman.cpp
  src/Server/server.cpp
  src/Server/starter.cpp
)

set(SERVER_HEADERS
  src/Memory_Manager/memman.h
  src/Server/server.h
)

add_executable(main
  ${SERVER_SOURCES}
  ${SERVER_HEADERS}
  ${memman_proto_srcs}
  ${memman_grpc_srcs}
)

target_link_libraries(main
  memman_proto
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  Threads::Threads
)

target_include_directories(main PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Memory_Manager
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Server
  ${CMAKE_CURRENT_SOURCE_DIR}/gen
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(CLIENT_SOURCES
    src/client.cpp
)

set(CLIENT_HEADERS
    src/MPointers/MPointers.h
)

add_executable(client
    ${CLIENT_SOURCES}
    ${CLIENT_HEADERS}
    ${memman_proto_srcs}
    ${memman_grpc_srcs}
)

target_link_libraries(client
    memman_proto
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
    Threads::Threads
)

target_include_directories(client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MPointers
    ${CMAKE_CURRENT_SOURCE_DIR}/gen
    ${CMAKE_CURRENT_BINARY_DIR}
)