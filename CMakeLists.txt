cmake_minimum_required(VERSION 3.20)

project(SandClock)

set(CMAKE_CXX_STANDARD 20)

set(TARGET SandClock)

set(SFML_DIR ${CMAKE_CURRENT_LIST_DIR}/Dependencies/SFML-2.6.1/lib/cmake/SFML)
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML COMPONENTS graphics system window)

set(SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Input.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Scene.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Time.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Tile.cpp
)

add_executable(${TARGET} ${SOURCES})

target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Dependencies/SFML-2.6.1/include)

target_link_libraries(${TARGET} PRIVATE sfml-graphics sfml-system sfml-window)
