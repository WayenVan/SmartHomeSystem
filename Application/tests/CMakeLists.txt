add_executable(servo_test servo_test.cc)

target_link_libraries(servo_test
    modules
    pigpio
    rt 
    pthread
)

target_include_directories(servo_test PUBLIC
    ${PROJECT_SOURCE_DIR}/Modules/Inc
    ${PROJECT_SOURCE_DIR}/Util
)

add_test(NAME ServoTest COMMAND servo_test)


#----------Servo_Test-----------#
add_executable(server_test server_test.cc)

target_link_libraries(server_test
    modules
    pthread
)

target_include_directories(server_test PUBLIC
    ${PROJECT_SOURCE_DIR}/Modules/Inc
    ${PROJECT_SOURCE_DIR}/Util
)

add_test(NAME ServerTest COMMAND server_test)