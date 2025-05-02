cmake_minimum_required(VERSION 3.30)
project(MicSpammer)

set(CMAKE_CXX_STANDARD 20)

# Enable AUTOMOC and ensure correct include dirs
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(MicSpammer
        main.cpp
        gui.h
        gui.cpp
)

set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")

# Add wxWidgets directories
set(Qt6_DIR "C:/Qt/6.9.0/mingw_64/lib/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

#include_directories(${wxWidgets_DIR}/include)
# Link the WASAPI libraries
target_link_libraries(MicSpammer PRIVATE
        Qt6::Core               # Qt
        Qt6::Gui
        Qt6::Widgets
        Ole32.lib               # WASAPI dependencies
        Uuid.lib
)

# Post-build step: copy Qt DLLs to the build directory
add_custom_command(
        TARGET MicSpammer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:MicSpammer>/platforms
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIB_DIR}/qwindows.dll"
        $<TARGET_FILE_DIR:MicSpammer>/platforms
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIB_DIR}/Qt6Core.dll"
        $<TARGET_FILE_DIR:MicSpammer>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIB_DIR}/Qt6Widgets.dll"
        $<TARGET_FILE_DIR:MicSpammer>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIB_DIR}/Qt6Gui.dll"
        $<TARGET_FILE_DIR:MicSpammer>
        COMMENT "Copying Qt DLLs and platform plugin to build directory"
)

#-DCMAKE_VERBOSE_MAKEFILE=ON
