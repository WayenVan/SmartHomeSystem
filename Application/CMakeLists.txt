cmake_minimum_required(VERSION 3.12.4)

project(VideoStream)

add_executable(${PROJECT_NAME} main.cc)

#opencv lib
find_package(OpenCV REQUIRED)

#modules library
add_subdirectory(Modules)

#myUtils library
add_subdirectory(Util)

target_link_libraries(${PROJECT_NAME}
    modules
    myUtils
    ${OpenCV_LIBS}
    python3.7m
)


# include dirctories
target_include_directories(${PROJECT_NAME} PUBLIC
    ${OpenCV_INCLUDE_DIRS}
    Modules/Inc
)

#-----------------------test------------------------#
add_subdirectory(tests)
enable_testing()


