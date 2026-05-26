cmake_minimum_required(VERSION 3.20)

project(WeatherApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опция для включения тестов
option(BUILD_TESTS "Build unit tests" ON)

include(FetchContent)

# Настройки для CPR
set(CPR_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CPR_FORCE_USE_SYSTEM_CURL OFF CACHE BOOL "" FORCE)
set(CPR_USE_SYSTEM_CURL OFF CACHE BOOL "" FORCE)
set(CPR_ENABLE_SSL ON CACHE BOOL "" FORCE)

FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 1.11.0
)

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_MakeAvailable(cpr)
FetchContent_MakeAvailable(json)

# Основная библиотека приложения
add_library(WeatherAppLib STATIC
        api.cpp
        api.h
        weather.cpp
        weather.h
        utils.cpp
        utils.h
)

target_link_libraries(WeatherAppLib PRIVATE
        cpr::cpr
        nlohmann_json::nlohmann_json
)

target_include_directories(WeatherAppLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Исполняемый файл приложения
add_executable(WeatherApp
        main.cpp
)

target_link_libraries(WeatherApp PRIVATE WeatherAppLib)

if (WIN32)
    add_custom_command(TARGET WeatherApp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:WeatherApp>
            $<TARGET_FILE_DIR:WeatherApp>
            COMMAND_EXPAND_LISTS
    )
endif()

if (MSVC)
    target_compile_options(WeatherApp PRIVATE /utf-8)
    target_compile_options(WeatherAppLib PRIVATE /utf-8)
endif()

# Юнит тесты
if(BUILD_TESTS)
    enable_testing()
    
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.15.2
    )
    
    FetchContent_MakeAvailable(googletest)
    
    add_executable(WeatherAppTests
            tests/test_api.cpp
            tests/test_weather.cpp
    )
    
    target_link_libraries(WeatherAppTests PRIVATE
            WeatherAppLib
            gtest_main
            cpr::cpr
            nlohmann_json::nlohmann_json
    )
    
    if(MSVC)
        target_compile_options(WeatherAppTests PRIVATE /utf-8)
    endif()
    
    include(GoogleTest)
    gtest_discover_tests(WeatherAppTests)
endif()