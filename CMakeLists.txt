cmake_minimum_required(VERSION 3.20)
project(AOC2025)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(PROJECT_INCLUDE_DIR "inc")
add_library(utils src/utils.cpp)
target_include_directories(utils PUBLIC ${PROJECT_INCLUDE_DIR})

# # set input directory
add_compile_definitions(INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/input")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

function(add_day DAY)
    set(PART1 src/day${DAY}/part1.cpp)
    set(PART2 src/day${DAY}/part2.cpp)
    set(LOGGING_DEFINITION PRIVATE $<$<CONFIG:Debug>:ENABLE_LOGGING>)

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${PART1})
        add_executable(day${DAY}_part1 ${PART1})

        target_link_libraries(day${DAY}_part1 utils)
        target_include_directories(day${DAY}_part1 PUBLIC ${PROJECT_INCLUDE_DIR})
        target_compile_definitions(day${DAY}_part1 ${LOGGING_DEFINITION})
    endif()

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${PART2})
        add_executable(day${DAY}_part2 ${PART2})

        target_link_libraries(day${DAY}_part2 utils)
        target_include_directories(day${DAY}_part2 PUBLIC ${PROJECT_INCLUDE_DIR})
        target_compile_definitions(day${DAY}_part2 ${LOGGING_DEFINITION})
    endif()
endfunction()

add_day(01)
add_day(02)
add_day(03)
add_day(04)
add_day(05)
add_day(06)
