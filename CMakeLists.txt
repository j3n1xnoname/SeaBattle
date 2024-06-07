cmake_minimum_required(VERSION 3.5.0)
project(BattleShip VERSION 0.1.0 LANGUAGES C CXX)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)



set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET)


set(SOURCE_FILES src/main.cpp src/Game.cpp src/Player.cpp src/Ship.cpp)
set(HEADER_FILES src/Game.hpp src/Enum.hpp src/Player.hpp src/Ship.hpp)

add_executable(BattleShip ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(BattleShip PRIVATE
    ${SDL2MIXER_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})

target_link_libraries(BattleShip PRIVATE raylib
    ${SDL2MIXER_LIBRARIES} ${SDL2_LIBRARIES})
