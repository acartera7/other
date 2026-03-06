cmake_minimum_required(VERSION 3.30)
project(MicSpammer)

set(CMAKE_CXX_STANDARD 20)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Common flags for all builds
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
# Debug-specific flas
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
# Optional: Release-specific flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Enable AUTOMOC and ensure correct include dirs
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
set(Qt6_INSTALL_DIR "C:/Qt/6.10.1/mingw_64" CACHE PATH "Path to Qt installation")
set(DLL_DIR "${Qt6_INSTALL_DIR}/bin")
set(PLAT_DIR "${Qt6_INSTALL_DIR}/plugins/platforms")
set(Qt6_DIR "${Qt6_INSTALL_DIR}/lib/cmake/Qt6")

# Paths for plugins
set(MULTIMEDIA_DIR "${Qt6_INSTALL_DIR}/plugins/multimedia")

find_package(OpenMP)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)

# Compile the resource file
qt_add_resources(app_resources resources.qrc)

add_executable(MicSpammer
        main.cpp
        MicSpammerWindow.h
        MicSpammerWindow.cpp
        FileBrowserWidget.cpp
        FileBrowserWidget.h
        AudioPlayer.cpp
        AudioPlayer.h
        WasapiManager.cpp
        WasapiManager.h
        MediaManager.cpp
        MediaManager.h
        NumpadWidget.cpp
        NumpadWidget.h
        ${app_resources}
        AudioLoader.cpp
        AudioLoader.h
        SoundInstance.cpp
        SoundInstance.h
        MicCapture.cpp
        MicCapture.h
        ProfileManager.cpp
        ProfileManager.h
)

# Link the WASAPI libraries
target_link_libraries(MicSpammer PRIVATE
        Qt6::Core               # Qt
        Qt6::Gui
        Qt6::Widgets
        Qt6::Multimedia
        Ole32.lib               # WASAPI dependencies
        ksuser.lib              # WAVEFORMATEXTENSIBLE
        Uuid.lib
        mfplat
        mf
        mfreadwrite
        mfuuid
)

# Collect FFmpeg DLLs
file(GLOB FFMPEG_DLLS
    "${DLL_DIR}/avcodec-*.dll"
    "${DLL_DIR}/avformat-*.dll"
    "${DLL_DIR}/avutil-*.dll"
    "${DLL_DIR}/swresample-*.dll"
    "${DLL_DIR}/swscale-*.dll"
)

# Post-build step: copy Qt DLLs to the build directory
add_custom_command(
        TARGET MicSpammer POST_BUILD

        # Create plugin directories
        COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:MicSpammer>/platforms
        COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:MicSpammer>/multimedia

        # Copy platform plugin
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PLAT_DIR}/qwindows.dll"
        $<TARGET_FILE_DIR:MicSpammer>/platforms

        # Copy Qt core DLLs
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DLL_DIR}/Qt6Core.dll"
        $<TARGET_FILE_DIR:MicSpammer>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DLL_DIR}/Qt6Widgets.dll"
        $<TARGET_FILE_DIR:MicSpammer>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DLL_DIR}/Qt6Gui.dll"
        $<TARGET_FILE_DIR:MicSpammer>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DLL_DIR}/Qt6Multimedia.dll"
        $<TARGET_FILE_DIR:MicSpammer>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DLL_DIR}/Qt6Network.dll"
        $<TARGET_FILE_DIR:MicSpammer>

        # Copy FFmpeg backend plugin
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${MULTIMEDIA_DIR}/ffmpegmediaplugin.dll"
        $<TARGET_FILE_DIR:MicSpammer>/multimedia
        #COMMAND ${CMAKE_COMMAND} -E copy_if_different
        #"${MULTIMEDIA_DIR}/fwindowsmediaplugin.dll"
        #$$<TARGET_FILE_DIR:MicSpammer>/multimedia

        #COMMAND ${CMAKE_COMMAND} -E copy_if_different
        #"${DLL_DIR}/avcodec-*.dll"
        #$<TARGET_FILE_DIR:MicSpammer>
        #COMMAND ${CMAKE_COMMAND} -E copy_if_different
        #"${DLL_DIR}/avformat-*.dll"
        #$<TARGET_FILE_DIR:MicSpammer>
        #COMMAND ${CMAKE_COMMAND} -E copy_if_different
        #"${DLL_DIR}/avutil-*.dll"
        #$<TARGET_FILE_DIR:MicSpammer>
        #COMMAND ${CMAKE_COMMAND} -E copy_if_different
        #"${DLL_DIR}/swresample-*.dll"
        #$<TARGET_FILE_DIR:MicSpammer>
        #COMMAND ${CMAKE_COMMAND} -E copy_if_different
        #"${DLL_DIR}/swscale-*.dll"
        #$<TARGET_FILE_DIR:MicSpammer>

        COMMENT "Copying Qt DLLs, FFmpeg backend, and audio plugins"
)


foreach (dll ${FFMPEG_DLLS})
    add_custom_command(
            TARGET MicSpammer POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${dll}"
            $<TARGET_FILE_DIR:MicSpammer>

            COMMENT "Copying Backend DLLs, FFmpeg backend"
    )
endforeach ()

#-DCMAKE_VERBOSE_MAKEFILE=ON
