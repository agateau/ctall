add_library(ctalllib STATIC
    Assets.cpp
    Background.cpp
    Bonus.cpp
    CtallApp.cpp
    GameScreen.cpp
    GameObject.cpp
    Player.cpp
    ScrollComponent.cpp
    Scroller.cpp
    StartScreen.cpp
    tools/App.cpp
    tools/Mask.cpp
    tools/MaskedTexture.cpp
    tools/Menu.cpp
    tools/Screen.cpp
    tools/TextDrawer.cpp
    Wall.cpp
    World.cpp
    WorldImpl.cpp
)

target_include_directories(ctalllib PUBLIC
    ${SDL2PP_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/tools
)
target_link_libraries(ctalllib ${SDL2PP_LIBRARIES})

add_executable(ctall main.cpp)
target_link_libraries(ctall ctalllib)
