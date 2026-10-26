add_library(funcsin SHARED funcsin.cpp)

target_include_directories(funcsin PRIVATE ${CMAKE_SOURCE_DIR}/Calculator)

set_target_properties(funcsin PROPERTIES
    PREFIX ""
    OUTPUT_NAME "funcsin"
    RUNTIME_OUTPUT_DIRECTORY         "${CMAKE_BINARY_DIR}/$<CONFIG>/plugins"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/Debug/plugins"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release/plugins"
)