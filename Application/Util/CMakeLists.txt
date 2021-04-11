add_library(myUtils my_utils.cc)

target_include_directories(myUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

