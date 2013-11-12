cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project("WeMo")

set(SOURCE_FILES WeMo.cpp Main.cpp)

set(HEADER_FILES win32_exception.h WeMo.h)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(WeMo ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(WeMo Winhttp.lib)
