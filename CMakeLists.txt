cmake_minimum_required(VERSION 3.0.0)

project(Quake-2-main)

set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE )
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message( "CMAKE BUILD TYPE: ${CMAKE_BUILD_TYPE}" )

macro(CmakeToWindow InputPath OutputPath)
    string(REGEX REPLACE "/" "\\\\" ${OutputPath} "${InputPath}")
endmacro()

set( QUAKE_2_BUILD_ROOT ${CMAKE_SOURCE_DIR} )
message( "QUAKE_2_BUILD_ROOT: ${QUAKE_2_BUILD_ROOT}" )
if( MSVC )
    set( QUAKE_2_OUTPUT_DIR ${QUAKE_2_BUILD_ROOT}/Build-Windows )
else()
    set( QUAKE_2_OUTPUT_DIR ${QUAKE_2_BUILD_ROOT}/Build-Linux )
endif()

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${QUAKE_2_OUTPUT_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${QUAKE_2_OUTPUT_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${QUAKE_2_OUTPUT_DIR} )
set( BUILD_SHARED_LIBS ON )
#add_subdirectory( "client" )
#add_subdirectory("qcommon")
add_subdirectory( "ref_gl" )
add_subdirectory( "shared" )
#add_subdirectory( "win32" )