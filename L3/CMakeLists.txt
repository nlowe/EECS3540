set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SOURCE_FILES_L3 Logger.cpp)

add_library(L3 ${SOURCE_FILES_L3})
target_link_libraries(L3 LINK_PUBLIC Threads::Threads)

target_include_directories(L3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})