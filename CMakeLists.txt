cmake_minimum_required(VERSION 3.17)
project(ctall)

include(GNUInstallDirs)
set(ASSETS_DIR ${CMAKE_INSTALL_FULL_DATADIR}/${PROJECT_NAME}/assets)

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
    add_compile_options(-Wall -Woverloaded-virtual)
endif()
set(CMAKE_CXX_STANDARD 17)

set(SDL2PP_WITH_IMAGE ON)
add_subdirectory(third-party)
add_subdirectory(src)
add_subdirectory(tests)

install(
    DIRECTORY assets/
    DESTINATION ${ASSETS_DIR}
)
