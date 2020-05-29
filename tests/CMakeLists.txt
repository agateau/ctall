add_executable(tests
    PoolTests.cpp
    tests.cpp)

target_include_directories(tests PRIVATE
    ${SDL2PP_INCLUDE_DIRS}
    ${ctall_SOURCE_DIR}/src/tools
)
target_link_libraries(tests ${SDL2PP_LIBRARIES} Catch2::Catch2)
