cmake_minimum_required(VERSION 3.15)
project(Async)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=thread -pthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -pthread")

add_executable(Async
  Async.hpp MessageLoop.cpp MessageLoop.hpp Utils.hpp Utils.cpp
  main.cpp)
add_executable(MessageLoopTest
  Async.hpp MessageLoop.cpp MessageLoop.hpp Utils.hpp Utils.cpp
  messageLoopTest.cpp)