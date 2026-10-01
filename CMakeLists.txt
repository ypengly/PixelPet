cmake_minimum_required(VERSION 3.24)
project(PixelPet LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 3.0.0 GIT_SHALLOW ON)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SFML)

add_executable(PixelPet WIN32
    src/main.cpp
    src/Windows/DesktopWindow.cpp
    src/Pet/PetRenderer.cpp)
target_include_directories(PixelPet PRIVATE src)
target_link_libraries(PixelPet PRIVATE SFML::Graphics SFML::Window SFML::System dwmapi)
