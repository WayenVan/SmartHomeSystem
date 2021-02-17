cmake_minimum_required(VERSION 3.9)
project(untitled1)

set(CMAKE_CXX_STANDARD 11)
add_executable(untitled1 main.cpp)

#find_library(OpenCV)
find_package(OpenCV REQUIRED)

target_include_directories(untitled1 PUBLIC
        ${OpenCV_INCLUDE_DIRS})

target_link_libraries(untitled1 ${OpenCV_LIBS})