cmake_minimum_required(VERSION 3.29)
project(Chip8)

set(CMAKE_CXX_STANDARD 17)

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