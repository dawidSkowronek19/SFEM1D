cmake_minimum_required(VERSION 3.14)
project(FemGrid CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- SZUKANIE PAKIETÓW ZEWNĘTRZNYCH ---
find_package(GSL REQUIRED)
find_package(Eigen3 REQUIRED) # Dodano wyszukiwanie biblioteki Eigen3

# 1. Stworzenie biblioteki rdzennej programu (bez funkcji main)
add_library(fem_core 
    src/Grid.cpp
    src/ShapeFunction.cpp
    src/Quadrature.cpp
    src/Physics.cpp
    src/Assembler.cpp
)
target_include_directories(fem_core PUBLIC src include)

# Podlinkowanie zewnętrznych zależności do naszej biblioteki
target_link_libraries(fem_core PUBLIC 
    GSL::gsl 
    GSL::gslcblas 
    m 
    Eigen3::Eigen
)

# 2. Główny plik wykonywalny programu
add_executable(fem_1d src/main.cpp)
target_link_libraries(fem_1d PRIVATE fem_core)

# 3. Pobranie Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# 4. Podpięcie folderu z testami
enable_testing()
add_subdirectory(tests)
