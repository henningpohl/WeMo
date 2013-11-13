cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project("WeMo")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE_FILES WeMo.cpp)
set(HEADER_FILES win32_exception.h WeMo.h)

add_executable(WeMoMain ${SOURCE_FILES} Main.cpp ${HEADER_FILES})
set_target_properties(WeMoMain PROPERTIES OUTPUT_NAME WeMo)
target_link_libraries(WeMoMain Winhttp.lib)

add_library(WeMoDll SHARED ${SOURCE_FILES} DllMain.cpp ${HEADER_FILES})
set_target_properties(WeMoDll PROPERTIES OUTPUT_NAME WeMo)
target_link_libraries(WeMoDll Winhttp.lib)