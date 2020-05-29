add_executable(ctall
    Assets.cpp
    Bonus.cpp
    Game.cpp
    GameObject.cpp
    main.cpp
    Player.cpp
    ScrollComponent.cpp
    Scroller.cpp
    tools/App.cpp
    tools/Pool.cpp
    tools/Screen.cpp
    tools/TextDrawer.cpp
    Wall.cpp
)

target_include_directories(ctall PRIVATE
    ${SDL2PP_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/tools
)
target_link_libraries(ctall ${SDL2PP_LIBRARIES})
