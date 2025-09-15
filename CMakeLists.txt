project(quake-2-main)

message("CMAKE BUILD TYPE: ${CMAKE_BUILD_TYPE}")

macro(CmakeToWindow InputPath OutputPath)
    string(REGEX REPLACE "/" "\\\\" ${OutputPath} "${InputPath}")
endmacro()

macro( add_sources_group SUBDIRS_LIST )
    foreach( dir ${SUBDIRS_LIST} )
        file( GLOB SOURCES ${dir}/*.h ${dir}/*.hpp ${dir}/*.c ${dir}/*.cpp )
        message( "Source group ${dir} for ${SOURCES}" )
        source_group( ${dir} FILES ${SOURCES} )
        list( APPEND SOURCE_FILES ${SOURCES} )
    endforeach( dir ${SUBDIRS_LIST} )
endmacro()

message( "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}" )
message( "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}" )
file( RELATIVE_PATH CUL_RENEGATE_DIR ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )
message( "CUL_RENEGATE_DIR: ${CUL_RENEGATE_DIR}" )

set( CMAKE_POSITION_INDEPENDENT_CODE ON )
set( Q2_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
set( CUL_TESTS_RUN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/runDir" )

set( ENV{Q2_ROOT_DIR} ${Q2_ROOT_DIR} )
set( ENV{CUL_DLL} ${CUL_TESTS_RUN_DIR} )

set( CUL_BUILD_ROOT ${CMAKE_SOURCE_DIR} )
message( "CUL_BUILD_ROOT: ${CUL_BUILD_ROOT}" )
if( MSVC )
    set( Q2_OUTPUT_DIR ${CUL_BUILD_ROOT}/Build-Windows )
else()
    set( Q2_OUTPUT_DIR ${CUL_BUILD_ROOT}/Build-Linux )
endif()

set( ENV{Q2_OUTPUT_DIR} ${Q2_OUTPUT_DIR} )
list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules" )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Q2_OUTPUT_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${Q2_OUTPUT_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Q2_OUTPUT_DIR} )

# Add sub directories
add_subdirectory(ctf)
add_subdirectory(game)
add_subdirectory(ref_gl)
add_subdirectory(ref_soft)
add_subdirectory(quake2)