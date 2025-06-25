cmake_minimum_required(VERSION 3.5.0)
project(BattleShip VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)

FetchContent_Declare(
        SDL2
		DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL "https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.32.8.tar.gz"
)
FetchContent_MakeAvailable(SDL2)

set(SDL2MIXER_VENDORED OFF)
set(SDL2MIXER_OPUS OFF)
set(SDL2MIXER_MOD OFF)
set(SDL2MIXER_MIDI OFF)
set(SDL2MIXER_WAVPACK OFF)
FetchContent_Declare(
        SDL2_mixer
		DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL "https://github.com/libsdl-org/SDL_mixer/archive/refs/tags/release-2.8.1.tar.gz"
)
FetchContent_MakeAvailable(SDL2_mixer)

set(RAYLIB_VERSION 5.0)
FetchContent_Declare(
	raylib
	DOWNLOAD_EXTRACT_TIMESTAMP OFF
	URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
)
FetchContent_GetProperties(raylib)
if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
	set(FETCHCONTENT_QUIET NO)
	FetchContent_MakeAvailable(raylib)
	set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
endif()

set(SOURCE_FILES src/main.cpp src/Game.cpp src/Player.cpp src/Ship.cpp)
set(HEADER_FILES src/Game.hpp src/Enum.hpp src/Player.hpp src/Ship.hpp)

add_executable(BattleShip ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(BattleShip PRIVATE
    raylib SDL2::SDL2main SDL2::SDL2 SDL2_mixer::SDL2_mixer)
