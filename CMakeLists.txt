cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(ShaderLab VERSION 1.0 LANGUAGES CXX)

set(OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/Debug")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/Debug")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/Release")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC On)

set(qt_components Core Concurrent Gui Multimedia Network OpenGL PrintSupport)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
        ${qt_components})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
        ${qt_components})
find_package(OpenGL REQUIRED)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

set(FFMPEG_DYNAMIC_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/lib/Linux/ffmpeg)
set(OPENGL_DYNAMIC_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/lib/Linux/OpenGL)
set(QSCI_DYNAMIC_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/lib/Linux/Qscintilla)

set(FFMPEG_LIBS
        ${FFMPEG_DYNAMIC_LIBS}/libavcodec.so
        ${FFMPEG_DYNAMIC_LIBS}/libavdevice.so
        ${FFMPEG_DYNAMIC_LIBS}/libavfilter.so
        ${FFMPEG_DYNAMIC_LIBS}/libavformat.so
        ${FFMPEG_DYNAMIC_LIBS}/libavutil.so
        ${FFMPEG_DYNAMIC_LIBS}/libswresample.so
        ${FFMPEG_DYNAMIC_LIBS}/libswscale.so)
set(OPENGL_LIBS
        ${OPENGL_DYNAMIC_LIBS}/libglad.so)
set(QSCI_LIBS
        ${QSCI_DYNAMIC_LIBS}/libqscintilla2_qt6.so)

set(DLL_FILES "")

file(GLOB_RECURSE FFMPEG_DLL_FILES "${FFMPEG_DYNAMIC_LIBS}/*.so")
list(APPEND DLL_FILES ${FFMPEG_DLL_FILES})
file(GLOB_RECURSE OPENGL_DLL_FILES "${GLAD_DYNAMIC_LIBS}/*.so")
list(APPEND DLL_FILES ${OPENGL_DLL_FILES})
file(GLOB_RECURSE QSCI_DLL_FILES "${QSCI_DYNAMIC_LIBS}/*.so")
list(APPEND DLL_FILES ${QSCI_DLL_FILES})
file(GLOB_RECURSE SDL_BIN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/bin/Linux/*")
list(APPEND DLL_FILES ${SDL_BIN_FILES})


set(DESTINATION_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

foreach(DLL_FILE ${DLL_FILES})
        file(COPY ${DLL_FILE} DESTINATION ${OUTPUT_DIR})
endforeach()
endif()


if(CMAKE_SYSTEM_NAME STREQUAL "Windows")

set(FFMPEG_DYNAMIC_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/lib/Windows/ffmpeg)
set(OPENGL_DYNAMIC_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/lib/Windows/OpenGL)
set(QSCI_DYNAMIC_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/lib/Windows/Qscintilla)

set(FFMPEG_LIBS
        ${FFMPEG_DYNAMIC_LIBS}/avcodec.lib
        ${FFMPEG_DYNAMIC_LIBS}/avdevice.lib
        ${FFMPEG_DYNAMIC_LIBS}/avfilter.lib
        ${FFMPEG_DYNAMIC_LIBS}/avformat.lib
        ${FFMPEG_DYNAMIC_LIBS}/avutil.lib
        ${FFMPEG_DYNAMIC_LIBS}/swresample.lib
        ${FFMPEG_DYNAMIC_LIBS}/swscale.lib)
set(OPENGL_LIBS
        ${OPENGL_DYNAMIC_LIBS}/glew32.lib
        ${OPENGL_DYNAMIC_LIBS}/glfw3.lib
        ${OPENGL_DYNAMIC_LIBS}/OpenGL32.Lib
        ${OPENGL_DYNAMIC_LIBS}/glad.lib
        )
set(QSCI_DEBUG_LIBS
        ${QSCI_DYNAMIC_LIBS}/qscintilla2_qt6d.lib)
set(QSCI_RELEASE_LIBS
        ${QSCI_DYNAMIC_LIBS}/qscintilla2_qt6.lib)

set(DLL_FILES "")

file(GLOB_RECURSE FFMPEG_DLL_FILES "${FFMPEG_DYNAMIC_LIBS}/*.dll")
list(APPEND DLL_FILES ${FFMPEG_DLL_FILES})
file(GLOB_RECURSE OPENGL_DLL_FILES "${OPENGL_LIBS}/*.dll")
list(APPEND DLL_FILES ${OPENGL_DLL_FILES})
file(GLOB_RECURSE QSCI_DLL_FILES "${QSCI_DYNAMIC_LIBS}/*.dll")
list(APPEND DLL_FILES ${QSCI_DLL_FILES})
file(GLOB_RECURSE SDL_BIN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/bin/Windows/*")
list(APPEND DLL_FILES ${SDL_BIN_FILES})


set(DESTINATION_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

foreach(DLL_FILE ${DLL_FILES})
        file(COPY ${DLL_FILE} DESTINATION ${OUTPUT_DIR})
endforeach()
endif()


foreach (_qt_comp IN LISTS qt_components)
    list(APPEND qt_modules "Qt${QT_VERSION_MAJOR}::${_qt_comp}")
endforeach ()

include_directories(src)
include_directories(include)
include_directories(include/ffmpeg)

file(GLOB_RECURSE UI_MODULE src/UI/*.h src/UI/*.cpp)
file(GLOB_RECURSE UTILITY_MODULE src/Utility/*.h src/Utility/*.cpp)
file(GLOB_RECURSE GL_MODULE src/GL/*.h src/GL/*.cpp)

add_executable(ShaderLab
        src/main.cpp
        src/ShaderLab.cpp
        src/SessionManager.h
        ${UI_MODULE}
        ${UTILITY_MODULE}
        ${GL_MODULE}
        )

link_directories(${FFMPEG_DYNAMIC_LIBS} ${OPENGL_LIBS} ${QSCI_DYNAMIC_LIBS})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
target_link_libraries(ShaderLab 
                debug ${GL_LIBRARY} ${qt_modules} ${FFMPEG_LIBS} ${OPENGL_LIBS} ${QSCI_DEBUG_LIBS}
        )
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
target_link_libraries(ShaderLab 
                optimized ${GL_LIBRARY} ${qt_modules} ${FFMPEG_LIBS} ${OPENGL_LIBS} ${QSCI_RELEASE_LIBS}
        )
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources "${CMAKE_CURRENT_BINARY_DIR}/Resources")
