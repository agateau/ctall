add_executable(ctall
    Assets.cpp
    main.cpp
    Game.cpp
    GameObject.cpp
    Player.cpp
    Pool.cpp
    ScrollComponent.cpp
    Scroller.cpp
    TextDrawer.cpp
    Wall.cpp
)

target_include_directories(ctall PRIVATE ${SDL2PP_INCLUDE_DIRS})
target_link_libraries(ctall ${SDL2PP_LIBRARIES})
