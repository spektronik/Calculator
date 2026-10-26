add_library(funcln SHARED funcln.cpp)

target_include_directories(funcln PRIVATE ${CMAKE_SOURCE_DIR}/Calculator)

set_target_properties(funcln PROPERTIES
    PREFIX ""
    OUTPUT_NAME "funcln"
    RUNTIME_OUTPUT_DIRECTORY         "${CMAKE_BINARY_DIR}/$<CONFIG>/plugins"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/Debug/plugins"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release/plugins"
)