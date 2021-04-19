add_library(myUtils
    my_utils.cc
    algorithm.cc
)

target_link_libraries(myUtils
    ${OpenCV_LIBS}
)

target_include_directories(myUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

