cmake_minimum_required(VERSION 3.19.4)

project(VideoStream)

add_executable(${PROJECT_NAME} main.cc)

find_package(OpenCV REQUIRED)

if(OpenCV_NOT_FOUND)
    message((FATAL_ERROR "opencv library not found"))
endif()

# include dirctories
target_include_directories(${PROJECT_NAME} PUBLIC
    ${OpenCV_INCLUDE_DIRS}
    Util 
    Modules/Inc
)
message(${OpenCV_INCLUDE_DIRS})


#libraries
target_link_libraries(${PROJECT_NAME} 
    ${OpenCV_LIBS}
)
message(${OpenCV_LIBS})


