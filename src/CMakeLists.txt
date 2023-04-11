cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ushell LANGUAGES C)

include(CTest)
include(FetchContent)

add_executable(${PROJECT_NAME}
    main.c
    ushell.h
    ushell.c
)

FetchContent_Declare(
    Unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.5.2
)

FetchContent_MakeAvailable(Unity)
add_library(Unity STATIC)
target_include_directories(Unity PUBLIC ${unity_SOURCE_DIR}/src)
target_sources(Unity PRIVATE
    ${unity_SOURCE_DIR}/src/unity.h
    ${unity_SOURCE_DIR}/src/unity.c
    ${unity_SOURCE_DIR}/src/unity_internals.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE Unity)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_BINARY_DIR}/../compile_commands.json)

add_test(unity_test ${PROJECT_NAME})

if(MSVC)
    target_compile_options(ushell PRIVATE /W4)
else()
    target_compile_options(ushell PRIVATE -Wall -Wextra -pedantic)
    set(SANITIZERS "-fsanitize=address,undefined,leak")
    target_compile_options(ushell PRIVATE "$<$<CONFIG:DEBUG>:${SANITIZERS}>")
    target_link_options(ushell PRIVATE "$<$<CONFIG:DEBUG>:${SANITIZERS}>")
endif()
