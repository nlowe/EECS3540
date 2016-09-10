# Set the default logging option
set(SOURCE_FILES_parcp util.cpp main.cpp copy.cpp)
add_executable(parcp ${SOURCE_FILES_parcp})

target_link_libraries(parcp LINK_PUBLIC L3)