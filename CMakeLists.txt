
cmake_minimum_required(VERSION 3.16)

project(Warship LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(headers)

file(GLOB SRC_FILES
    src/*.cpp
)

add_executable(warship ${SRC_FILES})

# Под линуксом нужно добавлять pthread + иногда dl
if(UNIX)
    target_link_libraries(warship PRIVATE pthread)
endif()

