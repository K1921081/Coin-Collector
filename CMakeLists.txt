cmake_minimum_required(VERSION 3.10)

project(CoinCollector)

add_executable(CoinCollector src/main.cpp
                             src/Physics.h
                             src/Constants.h
                             src/Game.cpp
                             src/Game.h
                             src/Object.cpp
                             src/Object.h
                             src/Texture.cpp
                             src/Texture.h
                             src/Player.cpp
                             src/Player.h
                             src/Camera.cpp
                             src/Camera.h)

# Add the SDL library to the project
if (WIN32)
    set(SDL2_DIR "<PathToSDL2>") # Change <PathToSDL2> to the path to the SDL2 library on your system
endif()

find_package(SDL2)

if (SDL2_FOUND)
    message("SDL2 found")
else()
    message("SDL2 not found")
endif()

target_include_directories(CoinCollector PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_directories(CoinCollector PUBLIC ${SDL2_LIBRARY_DIRS})
target_link_libraries(CoinCollector SDL2main SDL2)