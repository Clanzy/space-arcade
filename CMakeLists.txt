cmake_minimum_required(VERSION 3.0.0)
project(space-arcade VERSION 1.0.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

set(
    SOURCE_FILES src/main.cpp src/game_loop.hpp src/game_loop.cpp src/game_state.hpp
    src/numkeys.hpp src/renderer.hpp src/random.hpp src/random.cpp
    src/sdl_secure.hpp src/timer.hpp src/timer.cpp src/settings.hpp src/vector.hpp src/textures.hpp
    src/sc_start.hpp src/sc_start.cpp src/sc_gameplay.hpp src/sc_gameplay.cpp
    src/grid.hpp src/grid.cpp src/sc_game_over.hpp src/sc_game_over.cpp
    src/components.hpp src/entity.hpp src/entity.cpp src/system_render.hpp src/system_render.cpp
    src/system_movement.hpp src/system_movement.cpp src/system_input.hpp src/system_input.cpp
    src/system_script.hpp src/system_script.cpp src/system_shooting.hpp src/system_shooting.cpp
    src/system_sound.hpp src/system_sound.cpp
)

add_executable(space-arcade ${SOURCE_FILES})

target_link_libraries(space-arcade ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES})

target_link_directories(space-arcade PUBLIC
    ${SDL_LIBRARY_DIR}
    ${SDL_IMAGE_LIBRARY_DIR}
    ${SDL_MIXER_LIBRARY_DIR}
)

target_link_libraries(space-arcade
    SDL2
    SDL2main
    SDL2_image
    SDL2_mixer
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
