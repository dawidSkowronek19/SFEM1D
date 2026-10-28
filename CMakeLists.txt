cmake_minimum_required(VERSION 3.15)
project(sfem1d CXX)

# Wymuszamy C++17 (dla Eigena i pybind11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Szukamy Eigen3 w systemie
find_package(Eigen3 REQUIRED)

# ==========================================
# Pobieranie pybind11 w locie (FetchContent)
# ==========================================
include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG        v2.11.1
)
FetchContent_MakeAvailable(pybind11)

# ==========================================
# 1. Budowa rdzenia C++
# ==========================================
add_library(MyCoreLib STATIC
    src/ShapeFunction.cpp
)

set_target_properties(MyCoreLib PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Rdzeń musi widzieć nagłówki i Eigena
target_include_directories(MyCoreLib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(MyCoreLib PUBLIC Eigen3::Eigen)

# ==========================================
# 2. Budowa mostu Pythonowego
# ==========================================
# pybind11_add_module pochodzi z pobranego wyżej pybind11
pybind11_add_module(shape_module tests/bindings.cpp)

# Łączymy moduł z Twoim kodem C++
target_link_libraries(shape_module PRIVATE MyCoreLib)


