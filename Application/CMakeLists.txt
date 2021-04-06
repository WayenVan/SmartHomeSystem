cmake_minimum_required(VERSION 3.19.4)

project(VideoStream)

add_executable(${PROJECT_NAME} main.cc)

#opencv lib
find_package(OpenCV REQUIRED)
target_link_libraries(${PROJECT_NAME} 
    ${OpenCV_LIBS}
)

#modules library
add_subdirectory(Modules)
target_link_libraries(${PROJECT_NAME}
    modules
)


# include dirctories
target_include_directories(${PROJECT_NAME} PUBLIC
    ${OpenCV_INCLUDE_DIRS}
    Modules/Inc
)

#-----------------------test------------------------#
add_subdirectory(tests)
enable_testing()


