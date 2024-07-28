cmake_minimum_required(VERSION 3.14)
project(grpc_example)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

# 使用 FetchContent 下载 gRPC 和 Protobuf
include(FetchContent)

# 配置 Protobuf 本地路径
FetchContent_Declare(
  protobuf
  URL file://${CMAKE_CURRENT_SOURCE_DIR}/local/protobuf-26.1.tar.gz
  SOURCE_SUBDIR cmake
  OVERRIDE_FIND_PACKAGE
)

# 配置 gRPC 本地路径
FetchContent_Declare(
  grpc
  URL file://${CMAKE_CURRENT_SOURCE_DIR}/local/grpc-1.65.1.tar.gz
  SOURCE_SUBDIR cmake
  OVERRIDE_FIND_PACKAGE
)

# 关闭 protobuf 的测试
set(protobuf_BUILD_TESTS OFF CACHE INTERNAL "")

# 下载并构建 Protobuf 和 gRPC
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(protobuf grpc)

# 确保 gRPC 和 Protobuf 的配置文件可以找到
set(gRPC_DIR ${grpc_BINARY_DIR})
set(Protobuf_DIR ${protobuf_BINARY_DIR})

# 打印调试信息
message(STATUS "Protobuf source directory: ${protobuf_SOURCE_DIR}")
message(STATUS "Protobuf build directory: ${protobuf_BINARY_DIR}")
message(STATUS "gRPC source directory: ${grpc_SOURCE_DIR}")
message(STATUS "gRPC build directory: ${grpc_BINARY_DIR}")

# 查找 gRPC 和 Protobuf
# find_package(Protobuf REQUIRED)
# find_package(gRPC REQUIRED)

# 生成 gRPC 代码
set(PROTO_FILES
  proto/example.proto
)
set(PROTO_SRC_FILES
  ${CMAKE_BINARY_DIR}/proto/example.pb.cc
  ${CMAKE_BINARY_DIR}/proto/example.grpc.pb.cc
)
add_custom_command(
  OUTPUT ${PROTO_SRC_FILES}
  COMMAND ${Protobuf_PROTOC_EXECUTABLE}
  ARGS --grpc_out ${CMAKE_BINARY_DIR}/proto
       --cpp_out ${CMAKE_BINARY_DIR}/proto
       --proto_path ${CMAKE_CURRENT_SOURCE_DIR}/proto
       ${PROTO_FILES}
  DEPENDS ${PROTO_FILES}
  COMMENT "Generating gRPC code from ${PROTO_FILES}"
)

# 添加可执行文件
add_executable(grpc_client client.cpp ${PROTO_SRC_FILES})
add_executable(grpc_server server.cpp ${PROTO_SRC_FILES})

# 链接 gRPC 和 Protobuf 库
target_link_libraries(grpc_client
  PRIVATE
  grpc++_unsecure
  protobuf::libprotobuf
)
target_link_libraries(grpc_server
  PRIVATE
  grpc++_unsecure
  protobuf::libprotobuf
)

# 设置包含目录
target_include_directories(grpc_client
  PRIVATE
  ${CMAKE_BINARY_DIR}/proto
  ${gRPC_INCLUDE_DIRS}
  ${Protobuf_INCLUDE_DIRS}
)
target_include_directories(grpc_server
  PRIVATE
  ${CMAKE_BINARY_DIR}/proto
  ${gRPC_INCLUDE_DIRS}
  ${Protobuf_INCLUDE_DIRS}
)
