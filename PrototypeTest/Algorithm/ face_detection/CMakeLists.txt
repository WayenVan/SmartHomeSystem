cmake_minimum_required(VERSION 3.17)
project(FaceDetection)

set(CMAKE_CXX_STANDARD 14)

add_executable(FaceDetection main.cpp)

#opencv lib
find_package(OpenCV REQUIRED)

#libraries
target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBS}
        )

# include dirctories
target_include_directories(${PROJECT_NAME} PUBLIC
        ${OpenCV_INCLUDE_DIRS}
        )

