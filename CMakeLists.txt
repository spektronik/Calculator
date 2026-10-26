cmake_minimum_required (VERSION 3.10)

project(Calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /permissive- /EHsc)
endif()

add_executable(calc
    Calculator/main.cpp
    Calculator/Parser.cpp
    Calculator/PluginManager.cpp
)

target_include_directories(calc PRIVATE ${CMAKE_SOURCE_DIR}/Calculator)

set_target_properties(calc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY         "${CMAKE_BINARY_DIR}/$<CONFIG>"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release"
)

add_subdirectory(plugins/funcsin)
add_subdirectory(plugins/funcdeg)
add_subdirectory(plugins/funcln)
