add_library(modules 
  ${CMAKE_CURRENT_SOURCE_DIR}/Src/Detector.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Src/ServerAndroid.cc
)

#find boost 
find_package(Boost REQUIRED)
message("Boost Inc: ${Boost_INCLUDE_DIRS}")

#find threads
find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(m PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(modules "${CMAKE_THREAD_LIBS_INIT}")
endif()

#find opencv
find_package(OpenCV REQUIRED)
#libraries
target_link_libraries(modules
    ${OpenCV_LIBS}
)

target_include_directories(modules PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    ${PROJECT_SOURCE_DIR}/Util
    ${Boost_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)










