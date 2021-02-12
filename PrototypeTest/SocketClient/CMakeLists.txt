cmake_minimum_required(VERSION 3.19.4)

project(SocketClient)

add_executable(main main.cc)

#find opencv
find_package(OpenCV REQUIRED)
#libraries
target_link_libraries(main
    ${OpenCV_LIBS}
)
#include
target_include_directories(main PUBLIC
    ${OpenCV_INCLUDE_DIRS}
)