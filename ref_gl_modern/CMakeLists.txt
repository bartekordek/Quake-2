#cmake -DCMAKE_BUILD_TYPE=Debug
cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_C_STANDARD 90)
set(CMAKE_CXX_STANDARD 23)

set( PROJECT_NAME ref_gl_modern )

project(${PROJECT_NAME} LANGUAGES C CXX)

#find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

set( CMAKE_ENABLE_EXPORTS true )
message("INCLUDING: $ENV{GLEW_ROOT}")
include_directories($ENV{GLEW_ROOT}/include)

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
else()
    if(CMAKE_CXX_SIMULATE_ID MATCHES "MSVC")
        message(STATUS "WARNING: building clang-cl version.")
    else()
        message(STATUS "WARNING: building regular version.")
        # lots of warnings and all warnings as errors
        add_compile_options( -Wall -Wextra -Wno-unused-parameter )
        # -Wno-sized-deallocation
        add_compile_options( -Wno-unused-variable )
        add_definitions( -Wfatal-errors )
    endif()
endif()


file( GLOB_RECURSE SOURCES *.h *.c *.hpp *.cpp )

source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} )

list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/game/q_shared.c)
list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/win32/q_shwin.c)

add_library( ${PROJECT_NAME} SHARED ${SOURCES} )

set(DEPENDENCIES PRIVATE shared PRIVATE OpenGL::GL PRIVATE glew_s PRIVATE glm::glm )
if (MSVC)
list(APPEND DEPENDENCIES PRIVATE winmm )
endif()

set_property( TARGET ${PROJECT_NAME} PROPERTY "LINK_LIBRARY_DEPENDENCIES" e_true )

if (MSVC)
    list(APPEND LIBS msvcmrtd )
    target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL:NO")
endif()
target_link_libraries(${PROJECT_NAME} ${DEPENDENCIES})

if( MSVC )
else()
    if( NOT APPLE )
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    endif()
endif()

target_include_directories(${PROJECT_NAME} PUBLIC inc PRIVATE src)
target_compile_definitions(${PROJECT_NAME} PRIVATE C_ONLY=1 )