add_library(funcdeg SHARED funcdeg.cpp)

target_include_directories(funcdeg PRIVATE ${CMAKE_SOURCE_DIR}/Calculator)

set_target_properties(funcdeg PROPERTIES
    PREFIX ""
    OUTPUT_NAME "funcdeg"
    RUNTIME_OUTPUT_DIRECTORY         "${CMAKE_BINARY_DIR}/$<CONFIG>/plugins"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/Debug/plugins"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release/plugins"
)