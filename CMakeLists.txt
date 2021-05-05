cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

get_filename_component( basename ${CMAKE_CURRENT_SOURCE_DIR} NAME )
project( ${basename} LANGUAGES CXX )
set(CMAKE_CXX_STANDARD 17)

set( EXEC ${basename} )

file(MAKE_DIRECTORY Build-Windows)
file(MAKE_DIRECTORY Build-Linux)

set( QUAKE_2_BUILD_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )
if( MSVC )
    set( QUAKE_2_DIR ${QUAKE_2_BUILD_ROOT}/Build-Windows )
else()
    set( QUAKE_2_DIR ${QUAKE_2_BUILD_ROOT}/Build-Linux )
endif()

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${QUAKE_2_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${QUAKE_2_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${QUAKE_2_DIR} )

if( MSVC )
    # Disable warnings for time being. W4 to turn them on.
    add_compile_options( /W0 )
    add_compile_options( /MP )
else()
    #add_compile_options( -Wall -Wextra -pedantic -Werror )
    #add_definitions( -Wfatal-errors )
    add_compile_options( -w -fmax-errors=1 )
endif()

# DEPS
#add_subdirectory( ${QUAKE_2_BUILD_ROOT}/deps/OpenGLWrapper/deps/SDL2Wrapper/deps/CUL )

set( DIRECTORIES_LIST
client
game
qcommon
ref_gl
ref_soft
server
shared
)

if (MSVC)
    list( APPEND DIRECTORIES_LIST win32 )
else()
    list( APPEND DIRECTORIES_LIST linux )
endif()

set( SOURCES "" )
foreach(DIR ${DIRECTORIES_LIST})
    file( GLOB_RECURSE SOURCES_FROM_DIR ${DIR}/*.hpp ${DIR}/*.cpp )
    source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES_FROM_DIR} )
    list( APPEND SOURCES ${SOURCES_FROM_DIR} )
endforeach()

include_directories( "." )

if( MSVC )
    add_executable( ${EXEC} WIN32 ${SOURCES} )
else()
    add_executable( ${EXEC} ${SOURCES} )
endif()

#set( DEPENDENCIES cul )


if( MSVC )
    list( APPEND DEPENDENCIES Winmm.lib WSock32.lib )
endif()

if( MSVC )
    target_link_libraries( ${EXEC} ${DEPENDENCIES} )
else()
    target_link_libraries( ${EXEC} ${DEPENDENCIES} stdc++fs glut  )
endif()

if (MSVC)
    set_target_properties( ${EXEC} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/runDir )
endif()