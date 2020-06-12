# Define Catch2 as a header-only library, mimicking the interface produced by
# Catch2 cmake files when Catch2 is installed system-wide
add_library(Catch2 INTERFACE)
target_compile_definitions(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(Catch2::Catch2 ALIAS Catch2)

add_subdirectory(libSDL2pp)
# Forward variables
SET(SDL2PP_INCLUDE_DIRS ${SDL2PP_INCLUDE_DIRS} PARENT_SCOPE)
SET(SDL2PP_LIBRARIES ${SDL2PP_LIBRARIES} PARENT_SCOPE)

set(TMXLITE_STATIC_LIB ON)
add_subdirectory(tmxlite/tmxlite)
