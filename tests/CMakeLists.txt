# Tworzenie pliku wykonywalnego dla testów
add_executable(run_tests test_grid.cpp)

# Łączenie testów z Google Test oraz Twoją klasą Grid_1D
target_link_libraries(run_tests
    GTest::gtest_main
    grid_lib
)

# Rejestracja testów w systemie CTest
include(GoogleTest)
gtest_discover_tests(run_tests)
