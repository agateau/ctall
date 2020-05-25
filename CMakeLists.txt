cmake_minimum_required(VERSION 3.17)
project(ctall)

include(FindPkgConfig)
find_package(SDL2 REQUIRED)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

add_executable(ctall-sdl2
    Assets.cpp
    main.cpp
    GameObject.cpp
    Pool.cpp
    ScrollComponent.cpp
    MySDLUtils.cpp
)

target_include_directories(ctall-sdl2
    PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(ctall-sdl2
    ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
