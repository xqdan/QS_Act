cmake_minimum_required(VERSION 3.1)
project(QS)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pipe -fpic -pthread -std=c++11")
set(DEPENDS_INSTALL_DIR "${CMAKE_SOURCE_DIR}/depends")
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
find_package(gRPC REQUIRED)
include_directories(${_gRPC_INCLUDE_DIR})
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
include(cmake/depends/http_parser.cmake)
include(cmake/depends/libevent.cmake)
include(cmake/depends/picojson.cmake)
include(cmake/depends/rapidjson.cmake)
include(cmake/depends/glog.cmake)
include_directories(${DEPENDS_INSTALL_DIR}/include)
link_directories(${DEPENDS_INSTALL_DIR}/lib)
include_directories(3rdparty/stout/include)
set(ENABLE_LIBEVENT 1)

set(PROCESS_SRC
3rdparty/libprocess/src/authenticator.cpp
3rdparty/libprocess/src/authenticator_manager.cpp
3rdparty/libprocess/src/clock.cpp
3rdparty/libprocess/src/firewall.cpp
3rdparty/libprocess/src/grpc.cpp
3rdparty/libprocess/src/gtest_constants.cpp
3rdparty/libprocess/src/help.cpp
3rdparty/libprocess/src/http.cpp
3rdparty/libprocess/src/http_proxy.cpp
3rdparty/libprocess/src/io.cpp
3rdparty/libprocess/src/latch.cpp
3rdparty/libprocess/src/logging.cpp
3rdparty/libprocess/src/memory_profiler.cpp
3rdparty/libprocess/src/metrics/metrics.cpp
3rdparty/libprocess/src/mime.cpp
3rdparty/libprocess/src/pid.cpp
3rdparty/libprocess/src/process.cpp
3rdparty/libprocess/src/profiler.cpp
3rdparty/libprocess/src/reap.cpp
3rdparty/libprocess/src/socket.cpp
3rdparty/libprocess/src/subprocess.cpp
3rdparty/libprocess/src/time.cpp
3rdparty/libprocess/src/posix/subprocess.cpp
3rdparty/libprocess/src/posix/libevent/libevent.cpp
3rdparty/libprocess/src/posix/libevent/libevent_poll.cpp
3rdparty/libprocess/src/posix/io.cpp
3rdparty/libprocess/src/posix/poll_socket.cpp
)
include_directories(3rdparty/libprocess/src)
include_directories(3rdparty/libprocess/include)

add_library(process ${PROCESS_SRC})
target_link_libraries(process http_parser grpc event glog z event_pthreads)

add_dependencies(process libhttp_parser libevent picojson rapidjson libglog)

add_executable(run EXCLUDE_FROM_ALL src/main.cc)
target_link_libraries(run process)
