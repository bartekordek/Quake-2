cmake_minimum_required(VERSION 3.5)

project(quake-2-main)

option(ENABLE_ADDRESS_SANITIZER "Enable AddressSanitizer" OFF)

if(ENABLE_ADDRESS_SANITIZER)
    if(MSVC)
        set(ADDRESS_SANITIZER_FLAG "/fsanitize=address")
    elseif(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang" AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(ADDRESS_SANITIZER_FLAG "-fsanitize=address")
    else()
        message(FATAL_ERROR "AddressSanitizer is not supported by this compiler")
    endif()

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ADDRESS_SANITIZER_FLAG}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDRESS_SANITIZER_FLAG}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ADDRESS_SANITIZER_FLAG}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${ADDRESS_SANITIZER_FLAG}")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${ADDRESS_SANITIZER_FLAG}")
endif()

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
set( Q2_BUILD_ROOT ${CMAKE_SOURCE_DIR} )

if( MSVC )
    set( Q2_OUTPUT_DIR "${CMAKE_BINARY_DIR}/$<CONFIG>" )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Q2_OUTPUT_DIR} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${Q2_OUTPUT_DIR} )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Q2_OUTPUT_DIR} )
endif()

list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules" )

# Add sub directories

set(ENV{GLEW_ROOT} ${CMAKE_CURRENT_SOURCE_DIR}/deps/glew-2.1.0)

set(GLEW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLEW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLEW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLEW_BUILD_UTILS OFF CACHE BOOL "" FORCE)
set(GLEW_STATIC ON CACHE BOOL "" FORCE)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
add_subdirectory($ENV{GLEW_ROOT}/build/cmake)


add_subdirectory(ctf)


add_subdirectory(game)
add_subdirectory(mathlib)
add_subdirectory(quake2)
add_subdirectory(ref_gl)
add_subdirectory(ref_gl_modern)
add_subdirectory(ref_soft)
add_subdirectory(shared)

set( SDL2_INCLUDE deps/SDL/include )
set(SDLVIDEO ON CACHE BOOL "REPLACE GLUT" FORCE)
add_subdirectory(deps/SDL)



add_subdirectory(deps/glm)