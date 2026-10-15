cmake_minimum_required(VERSION 3.14)
project(FemGrid CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Stworzenie biblioteki z Twojego kodu (aby testy mogły z niej korzystać)
add_library(grid_lib src/Grid.cpp)
target_include_directories(grid_lib PUBLIC src)

find_package(GSL REQUIRED)
target_link_libraries(grid_lib PUBLIC GSL::gsl GSL::gslcblas m)

# 2. Pobranie Google Test (nowoczesne podejście FetchContent)
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# 3. Podpięcie folderu z testami
enable_testing()
add_subdirectory(tests)
