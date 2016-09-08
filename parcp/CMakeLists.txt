# Set the default logging option
add_definitions(-DPARCP_DEFAULT_LOG_TRACE)

set(SOURCE_FILES_parcp main.cpp)
add_executable(parcp ${SOURCE_FILES_parcp})

target_link_libraries(parcp L3)