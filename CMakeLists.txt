
# cmake config
cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_TOOLCHAIN_FILE ./vcpkg/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# project config

project(intercept CXX)

add_executable(intercept
    
    src/main.cpp
)

# dependencies

find_package(fmt REQUIRED)
target_link_libraries(intercept PUBLIC fmt::fmt)

find_package(glm REQUIRED)
target_link_libraries(intercept PUBLIC glm::glm)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(intercept PRIVATE sfml-system sfml-network sfml-graphics sfml-window)


# flags

if ((NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") AND (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
    message(FATAL "unsupported compiler")
endif()
 

target_link_libraries(intercept PRIVATE

    $<$<CONFIG:RELEASE>:-flto>

    $<$<CONFIG:DEBUG>:-fsanitize=address>
    $<$<CONFIG:DEBUG>:-fsanitize=undefined>
)

target_compile_options(intercept PRIVATE

    -std=c++20
    -pedantic -Wall -Wextra
    -fdiagnostics-color=always

    $<$<CONFIG:DEBUG>:-g>

    $<$<CONFIG:RELEASE>:-DNDEBUG>
    $<$<CONFIG:RELEASE>:-O3>
    $<$<CONFIG:RELEASE>:-flto>
    $<$<CONFIG:RELEASE>:-march=native>
)
