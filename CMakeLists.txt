cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE )
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

get_filename_component( basename ${CMAKE_CURRENT_SOURCE_DIR} NAME )
project( ${basename} LANGUAGES C )
set( EXEC ${basename} )

file(MAKE_DIRECTORY Build-Windows)
file(MAKE_DIRECTORY Build-Linux)

if( MSVC )
    # Disable warnings for time being. W4 to turn them on.
    add_compile_options( /W0 )
    #add_compile_options( /MP )
else()
    #add_compile_options( -Wall -Wextra -pedantic -Werror )
    #add_definitions( -Wfatal-errors )
    add_compile_options( -w -fmax-errors=1 )
endif()

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
    list( APPEND DIRECTORIES_LIST LinuxDeps )
endif()

set( SOURCES "" )
foreach(DIR ${DIRECTORIES_LIST})
    file( GLOB_RECURSE SOURCES_FROM_DIR ${DIR}/*.h ${DIR}/*.c )
    source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES_FROM_DIR} )
    list( APPEND SOURCES ${SOURCES_FROM_DIR} )
endforeach()


if( MSVC )
include_directories( "." )
else()
include_directories( "." )
include_directories( "LinuxDeps" )
endif()

if( MSVC )
    add_executable( ${EXEC} WIN32 ${SOURCES} )
else()
    add_executable( ${EXEC} ${SOURCES} )
endif()

set( DEPENDENCIES "" )

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