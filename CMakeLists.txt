cmake_minimum_required(VERSION 3.16)

project(warships VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Quick)

qt_standard_project_setup(REQUIRES 6.8)

file(GLOB SRC_FILES
    ./*.cpp
)

qt_add_executable(appwarships
    ${SRC_FILES}
)

target_link_libraries(appwarships
    PRIVATE
        Qt6::Widgets
        Qt6::Gui
        Qt6::Core
)

set_target_properties(appwarships PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS appwarships
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
