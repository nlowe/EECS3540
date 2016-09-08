# Set the default logging option
add_definitions(-DCOPYTREE_DEFAULT_LOG_TRACE)

set(SOURCE_FILES_CopyTree main.cpp)
add_executable(copytree ${SOURCE_FILES_CopyTree})

target_link_libraries(copytree L3)