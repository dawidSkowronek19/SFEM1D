add_executable(run_tests test_grid.cpp)

target_link_libraries(run_tests PRIVATE 
    fem_core
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)
