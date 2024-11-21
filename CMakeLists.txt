cmake_minimum_required(VERSION 3.29)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(Chip8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB INCLUDE_FILES src/*.h)

find_package(SDL2 CONFIG REQUIRED)

add_executable(Chip8 ${SOURCE_FILES} ${INCLUDE_FILES})

target_include_directories(Chip8 PRIVATE include)
target_link_libraries(Chip8
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)