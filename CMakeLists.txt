cmake_minimum_required(VERSION 3.15)
project(sfem1d CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 REQUIRED)

include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG        v2.11.1
)
FetchContent_MakeAvailable(pybind11)

add_library(MyCoreLib STATIC
    src/ShapeFunction.cpp
    src/Quadrature.cpp
)

set_target_properties(MyCoreLib PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(MyCoreLib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(MyCoreLib PUBLIC Eigen3::Eigen)

pybind11_add_module(sfem1d tests/bindings.cpp)


target_link_libraries(sfem1d PRIVATE MyCoreLib)


