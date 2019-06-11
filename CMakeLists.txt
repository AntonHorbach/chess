cmake_minimum_required(VERSION 3.0.0)
project(chess VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

add_executable(chess src/game/main.cpp src/game/game.cpp src/game/field.cpp
                src/game/TextureManager.cpp src/game/figure.cpp
                src/tools/vectorTools.cpp
                src/tools/movesGenerators.cpp
                src/game/player.cpp)
                
target_link_libraries(chess SDL2 SDL2_image)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
