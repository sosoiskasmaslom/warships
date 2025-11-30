cmake_minimum_required(VERSION 3.16)

project(warships VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Quick)

qt_standard_project_setup(REQUIRES 6.8)

# include_directories(headers)
file(GLOB SRC_FILES
    src/*.cpp
)

qt_add_executable(appwarships
    src/main.cpp
    src/gui/window.cpp
    headers/gui/window.h
    headers/point.h
    ${SRC_FILES}
)

target_link_libraries(appwarships PRIVATE
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core
)

target_include_directories(appwarships PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(appwarships PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
