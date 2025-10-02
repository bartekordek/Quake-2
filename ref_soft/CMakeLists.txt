#cmake -DCMAKE_BUILD_TYPE=Debug
cmake_minimum_required(VERSION 3.0.0)

set( CMAKE_C_STANDARD 90 )

set( PROJECT_NAME ref_soft )

project(${PROJECT_NAME} LANGUAGES C)

set( CMAKE_ENABLE_EXPORTS e_true )

if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
    message(STATUS "WARNING: building msvc version.")
    # warning level 4 and all warnings as errors
    add_compile_options( /Wall )
    #add_compile_options( /WX ) # error on warning.
    add_compile_options( /MP )
    add_compile_options( "/wd4266" )
    add_compile_options( "/wd4820" )
    add_compile_options( "/wd4710" )
    add_compile_options( "/wd5045" )

    find_package( RapidJSON REQUIRED )
    if( RapidJSON_FOUND )
        include_directories( SYSTEM ${RapidJSON_INCLUDE_DIRS} )
    endif( RapidJSON_FOUND )
else()
    if(CMAKE_CXX_SIMULATE_ID MATCHES "MSVC")
        message(STATUS "WARNING: building clang-cl version.")
    else()
        message(STATUS "WARNING: building regular version.")
        # lots of warnings and all warnings as errors
        add_compile_options( -Wall -Wextra -Werror -Wno-unused-parameter )
        # -Wno-sized-deallocation
        add_compile_options( -Wno-unused-variable )
        add_definitions( -Wfatal-errors )
    endif()
endif()


file( GLOB_RECURSE SOURCES *.h *.c )

source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} )

list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/game/q_shared.c)
list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/win32/q_shwin.c)
list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/win32/rw_ddraw.c)
list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/win32/rw_dib.c)
list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/win32/rw_imp.c)

add_library( ${PROJECT_NAME} SHARED ${SOURCES} )

set(DEPENDENCIES PUBLIC shared)
if (MSVC)
list(APPEND DEPENDENCIES winmm)
endif()

set_property( TARGET ${PROJECT_NAME} PROPERTY "LINK_LIBRARY_DEPENDENCIES" e_true )

if (MSVC)
    list(APPEND LIBS msvcmrtd )
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL:NO")
    target_link_libraries( ${PROJECT_NAME} ${DEPENDENCIES} )
else()
    target_link_libraries( ${PROJECT_NAME} ${DEPENDENCIES} )
endif()

if( MSVC )
else()
    if( NOT APPLE )
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    endif()
endif()

target_include_directories(${PROJECT_NAME} PUBLIC .)


