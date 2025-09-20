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

set( ENV{Q2_ROOT_DIR} ${Q2_ROOT_DIR} )
set(ENV{Q2_DEPS_ROOT} ${Q2_ROOT_DIR}/deps)

list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules" )

# Add sub directories
add_subdirectory(ctf)
add_subdirectory(game)
add_subdirectory(quake2)
add_subdirectory(ref_gl)
add_subdirectory(ref_soft)
add_subdirectory(shared)
add_subdirectory(win32)

set(SDL_STATIC ON CACHE BOOL "SDL Static" FORCE)
set(SDL_SHARED OFF CACHE BOOL "SDL Dynamic" FORCE)
add_subdirectory(deps/SDL)