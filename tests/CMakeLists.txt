add_executable(tests
    MaskTests.cpp
    PoolTests.cpp
    tests.cpp)

target_link_libraries(tests ctalllib Catch2::Catch2)
