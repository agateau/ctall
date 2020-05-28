cmake_minimum_required(VERSION 3.17)
project(ctall)

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
    add_compile_options(-Wall -Woverloaded-virtual)
endif()
set(CMAKE_CXX_STANDARD 17)

set(SDL2PP_WITH_IMAGE ON)
add_subdirectory(third-party)
add_subdirectory(src)
add_subdirectory(tests)
