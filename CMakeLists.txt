cmake_minimum_required(VERSION 3.17)
project(ctall)

find_package(SFML COMPONENTS graphics window system REQUIRED)

add_executable(ctall
    Assets.cpp
    main.cpp
    GameObject.cpp
    Pool.cpp
    ScrollComponent.cpp
)

target_link_libraries(ctall sfml-graphics)
