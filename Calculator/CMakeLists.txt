add_executable(calc
    main.cpp
    Parser.cpp
    PluginManager.cpp
)
target_include_directories(calc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set(PLUGIN_OUT_DIR ${CMAKE_BINARY_DIR}/$<CONFIG>/plugins)
add_custom_command(TARGET calc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PLUGIN_OUT_DIR}"
)