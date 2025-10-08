project( quake2 LANGUAGES C)

set(CMAKE_CXX_STANDARD 20)

set(${PROJECT_NAME})

if (MSVC)
    add_compile_options()
    add_compile_options( /MP )
else()
    if(CMAKE_CXX_SIMULATE_ID MATCHES "MSVC")
    else()
        add_compile_options(-Wall -Wextra -pedantic)
    endif()
endif()

file( GLOB_RECURSE SOURCES *.h *.c *.hpp *.cpp )
source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} )

list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/game/m_flash.c)
list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/game/q_shared.c)
list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/server/sv_ccmds.c)
list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/server/sv_ents.c)
list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/server/sv_game.c)
list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/server/sv_init.c)
list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/server/sv_main.c)
list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/server/sv_send.c)
list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/server/sv_user.c)
list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/server/sv_world.c)
list(APPEND SOURCES $ENV{Q2_ROOT_DIR}/win32/q_shwin.c)

set(DEPENDENCIES
PRIVATE game
PRIVATE ref_gl
PRIVATE ref_soft
PRIVATE shared
)

if (MSVC)
list(APPEND DEPENDENCIES PRIVATE winmm PRIVATE Ws2_32 )
endif()

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE CPUSTRING="x64" C_ONLY=1 WIN32 _WINDOWS )

if (MSVC)
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
            OUTPUT_NAME ${PROJECT_NAME}
            SUFFIX ".exe"
    )
else()
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
            OUTPUT_NAME ${PROJECT_NAME}
            SUFFIX ".bin"
    )
endif()

target_link_libraries(${PROJECT_NAME} ${DEPENDENCIES})

if (MSVC)
    set_target_properties( ${PROJECT_NAME} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO" )
    message("CMAKE_BINARY_DIR : ${CMAKE_BINARY_DIR}")
    add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/$(Configuration)/ $(LocalDebuggerWorkingDirectory)
            COMMENT "Copying data")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC inc PRIVATE src)