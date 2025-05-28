cmake_minimum_required(VERSION 3.30.0)
project(SDLshooter VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
add_compile_options(-Wall -g)

set(CMAKE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(${TARGET}
        src/Game.h
        src/Game.cpp
        src/Scene.h
        src/SceneMain.cpp
        src/SceneMain.h
        src/main.cpp
        src/Object.h
        src/Scene.cpp
        src/SceneTitle.h
        src/SceneTitle.cpp
        src/SceneEnd.h
        src/SceneEnd.cpp
)

target_link_libraries(${TARGET} 
                        SDL2::SDL2main 
                        SDL2::SDL2
                        SDL2_image::SDL2_image
                        SDL2_mixer::SDL2_mixer
                        SDL2_ttf::SDL2_ttf)
                        