cmake_minimum_required(VERSION 4.1.1)
project(smallgame LANGUAGES CXX )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCE  ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

set(INCLUDE include/)
file(GLOB_RECURSE INCLUDE_FILES ${INCLUDE}/*)

include(FetchContent)
FetchContent_Declare(BOX2D
    GIT_REPOSITORY https://github.com/erincatto/box2d
    GIT_TAG v3.1.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
    
FetchContent_MakeAvailable(BOX2D)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
    
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(TOML
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus
    GIT_TAG v3.4.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)

FetchContent_MakeAvailable(TOML)

FetchContent_Declare(SPDLOG
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG v1.17.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)

FetchContent_MakeAvailable(SPDLOG)

add_compile_definitions(SFML_NO_JOYSTICK)

add_executable(smallgame ${SOURCE})

target_include_directories(smallgame PRIVATE ${INCLUDE})
target_compile_features(smallgame PRIVATE cxx_std_17)
target_link_libraries(smallgame PRIVATE
    SFML::Graphics
    box2d::box2d
    tomlplusplus::tomlplusplus
    spdlog::spdlog
)