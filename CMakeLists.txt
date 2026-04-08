cmake_minimum_required(VERSION 4.1)
project(_WX)

set(CMAKE_CXX_STANDARD 20)

add_executable(_WX main.cpp
        include/weather.h
        src/weather.cpp
        src/api.cpp
        include/api.h
)
