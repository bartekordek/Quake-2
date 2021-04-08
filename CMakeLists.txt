cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

get_filename_component( basename ${CMAKE_CURRENT_SOURCE_DIR} NAME )
project( ${basename} LANGUAGES C  )
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
endif()

set( SOURCES "" )
foreach(DIR ${DIRECTORIES_LIST})
    file( GLOB_RECURSE SOURCES_FROM_DIR ${DIR}/*.h ${DIR}/*.c )
    source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES_FROM_DIR} )
    list( APPEND SOURCES ${SOURCES_FROM_DIR} )
endforeach()

include_directories( "." )

add_executable( ${EXEC} ${SOURCES} )

set( DEPENDENCIES "" )

if( MSVC )
    list( APPEND DEPENDENCIES Winmm.lib WSock32.lib )
endif()

if( MSVC )
    target_link_libraries( ${EXEC} ${DEPENDENCIES} )
else()
    target_link_libraries( ${EXEC} ${DEPENDENCIES} stdc++fs glut  )
endif()