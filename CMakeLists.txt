cmake_minimum_required(VERSION 3.5)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(
        spectools-ng
        VERSION 0.1
        DESCRIPTION "Spectools-ng is a wifi spectrum analyser built on top of the work of Michael Kershaw"
        LANGUAGES C CXX
)
add_subdirectory(src)
