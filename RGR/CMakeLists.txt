cmake_minimum_required(VERSION 3.10)
project(LoadTester C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Находим библиотеки
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)
pkg_check_modules(CJSON REQUIRED libcjson)
pkg_check_modules(CMOCKA REQUIRED cmocka)

# Основная программа
add_executable(load_tester main.c)
target_link_libraries(load_tester 
    ${CURL_LIBRARIES} 
    ${CJSON_LIBRARIES} 
    pthread
)
target_include_directories(load_tester PRIVATE 
    ${CURL_INCLUDE_DIRS} 
    ${CJSON_INCLUDE_DIRS}
)

# UNIT-тесты
add_executable(tests test.c)
target_link_libraries(tests
    m
    ${CURL_LIBRARIES} 
    ${CJSON_LIBRARIES} 
    ${CMOCKA_LIBRARIES}
    pthread
)
target_include_directories(tests PRIVATE 
    ${CURL_INCLUDE_DIRS} 
    ${CJSON_INCLUDE_DIRS}
    ${CMOCKA_INCLUDE_DIRS}
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config ${CMAKE_CURRENT_BINARY_DIR}/config COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/endpoints.json ${CMAKE_CURRENT_BINARY_DIR}/endpoints.json COPYONLY)
