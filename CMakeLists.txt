cmake_minimum_required(VERSION 3.5)

project(Scorewarrior LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    "${CMAKE_SOURCE_DIR}/models"
    "${CMAKE_SOURCE_DIR}/utils"
    )

file(GLOB sources
    "./*.cpp"
    "./*.hpp"
    "./models/*.cpp"
    "./models/*.hpp"
    "./utils/*.cpp"
    "./utils/*.hpp")

add_executable(Scorewarrior ${sources})
