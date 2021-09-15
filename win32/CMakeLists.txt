cmake_minimum_required(VERSION 3.0.0)

get_filename_component( basename ${CMAKE_CURRENT_SOURCE_DIR} NAME )
project( ${basename} LANGUAGES C )
set( EXEC ${basename} )

if( MSVC )
    string(REPLACE "/INCREMENTAL" "" tempFlag ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
    set( CMAKE_EXE_LINKER_FLAGS_DEBUG ${tempFlag} )
endif()


if( MSVC )
    # Disable warnings for time being. W4 to turn them on.
    add_compile_options( /W0 )
    #add_compile_options( /MP )
else()
    #add_compile_options( -Wall -Wextra -pedantic -Werror )
    #add_definitions( -Wfatal-errors )
    add_compile_options( -w -fmax-errors=1 )
endif()

file( GLOB_RECURSE SOURCES *.h *.c )
source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} )
add_library( ${EXEC} SHARED ${SOURCES} )

set_property( TARGET ${EXEC} PROPERTY "LINK_LIBRARY_DEPENDENCIES" true )

set( DEPENDENCIES
shared
)

if (MSVC)
    list(APPEND LIBS msvcmrtd )
    target_link_libraries( ${EXEC} ${DEPENDENCIES} )
else()
    target_link_libraries( ${EXEC} ${DEPENDENCIES} stdc++fs )
endif()