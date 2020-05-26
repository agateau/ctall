cmake_minimum_required(VERSION 3.17)
project(ctall)

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
    add_compile_options(-Wall -Woverloaded-virtual)
endif()
set(CMAKE_CXX_STANDARD 17)

set(SDL2PP_WITH_IMAGE ON)
add_subdirectory(third-party/libSDL2pp)

add_executable(ctall-sdl2
    Assets.cpp
    main.cpp
    Game.cpp
    GameObject.cpp
    Player.cpp
    Pool.cpp
    ScrollComponent.cpp
    Scroller.cpp
    Wall.cpp
)

target_include_directories(ctall-sdl2 PRIVATE ${SDL2PP_INCLUDE_DIRS})
target_link_libraries(ctall-sdl2 ${SDL2PP_LIBRARIES})
