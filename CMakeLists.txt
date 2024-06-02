cmake_minimum_required(VERSION 3.5.0)
project(BattleShip VERSION 0.1.0 LANGUAGES C CXX)

set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET)

set(SOURCE_FILES src/main.cpp src/Game.cpp src/Player.cpp src/Ship.cpp)
set(HEADER_FILES src/Game.hpp src/Enum.hpp src/Player.hpp src/Ship.hpp)

add_executable(BattleShip ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(BattleShip raylib)

