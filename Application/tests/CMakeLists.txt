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
    python3.7m
    pigpio
)

target_include_directories(server_test PUBLIC
    ${PROJECT_SOURCE_DIR}/Modules/Inc
    ${PROJECT_SOURCE_DIR}/Util
)

add_test(NAME ServerTest COMMAND server_test)

#----------LockControlTest--------#
add_executable(lock_control_test lock_control_test.cc)

target_link_libraries(lock_control_test
    modules
    pthread
)

target_include_directories(lock_control_test PUBLIC
    ${PROJECT_SOURCE_DIR}/Modules/Inc
    ${PROJECT_SOURCE_DIR}/Util
)

add_test(NAME LockControlTest COMMAND lock_control_test)

#-----------bme680_test__________3

add_executable(bme680_test bme680_test.cc)

target_link_libraries(bme680_test
    modules
    python3.7m
    myUtils
)

target_include_directories(bme680_test PUBLIC
    ${PROJECT_SOURCE_DIR}/Modules/Inc
    ${PROJECT_SOURCE_DIR}/Util
)

add_test(NAME BME680Test COMMAND bme680_test)

#---------server_universal_test----#

add_executable(server_universal_test server_universal_test.cc)

target_link_libraries(server_universal_test
    modules
    python3.7m
    pigpio
)

target_include_directories(server_universal_test PUBLIC
    ${PROJECT_SOURCE_DIR}/Modules/Inc
    ${PROJECT_SOURCE_DIR}/Util
)

add_test(NAME ServerUniversalTest COMMAND server_universal_test)

#------------amg8833_test---------#


add_executable(amg8833_test amg8833_test.cc)

target_link_libraries(amg8833_test
    python3.7m
    pigpio
)

target_include_directories(amg8833_test PUBLIC
    ${PROJECT_SOURCE_DIR}/Modules/Inc
    ${PROJECT_SOURCE_DIR}/Util
)

add_test(NAME AMG8833Test COMMAND amg8833_test)

#--------------detect_manager_test-------#
add_executable(detect_manager_test detect_manager_test.cc)

target_link_libraries(detect_manager_test
    python3.7m
    pigpio
    pthread
    modules
)

target_include_directories(detect_manager_test PUBLIC
    ${PROJECT_SOURCE_DIR}/Modules/Inc
    ${PROJECT_SOURCE_DIR}/Util
)

add_test(NAME DetectManagerTest COMMAND detect_manager_test)