cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

get_filename_component( basename ${CMAKE_CURRENT_SOURCE_DIR} NAME )
project( ${basename} LANGUAGES C  )
set( EXEC ${basename} )

set( DIRECTORIES_LIST
client
game
qcommon
ref_gl
shared
)

if (MSVC)
    list( APPEND ${DIRECTORIES_LIST} win32 )
endif()

set( SOURCES "" )
foreach(DIR ${DIRECTORIES_LIST})
    message( "DIR: ${DIR}" )
    file( GLOB_RECURSE SOURCES_FROM_DIR ${DIR}/*.h ${DIR}/*.c )
    message( "SOURCES_FROM_DIR: ${SOURCES_FROM_DIR}" )

    source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES_FROM_DIR} )
    list( APPEND SOURCES ${SOURCES_FROM_DIR} )
    message( "SOURCES: ${SOURCES}" )
endforeach()

include_directories( "." )

message( "SOURCES: ${SOURCES}" )
add_executable( ${EXEC} ${SOURCES} )