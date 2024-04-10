cmake_minimum_required(VERSION 3.27.0)
project(base_for_driver VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDART 17)

set (DRIVER_SRC ${CMAKE_SOURCE_DIR}/driver.h
                ${CMAKE_SOURCE_DIR}/driver.cpp)

set (GLOBAL_SRC ${CMAKE_SOURCE_DIR}/global.h
                ${CMAKE_SOURCE_DIR}/global.cpp)

add_executable(base_for_driver main.cpp
                               ${DRIVER_SRC}
                               ${GLOBAL_SRC})

if (WIN32)
    target_link_libraries(base_for_driver iconv Ws2_32)
    message("Build for Windows")
elseif (UNIX)
    target_link_libraries("Build for UNIX")
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
