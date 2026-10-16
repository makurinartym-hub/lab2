cmake_minimum_required(VERSION 3.20)
project(ip_filter_lab2 LANGUAGES CXX)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(ip_core INTERFACE)
target_include_directories(ip_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(MSVC)
    target_compile_options(ip_core INTERFACE /utf-8 /constexpr:steps2000000)
endif()

add_executable(ip_filter src/ip_filter.cpp)
target_link_libraries(ip_filter PRIVATE ip_core)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(ip_filter_tests tests/test_ip_filter.cpp)
target_link_libraries(ip_filter_tests PRIVATE ip_core GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(ip_filter_tests)
