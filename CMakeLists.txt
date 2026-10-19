cmake_minimum_required(VERSION 4.1.1)
project(smallgame LANGUAGES C CXX )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCE  ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

set(INCLUDE include/)
file(GLOB_RECURSE INCLUDE_FILES ${INCLUDE}/*)

include(FetchContent)
FetchContent_Declare(BOX2D
    GIT_REPOSITORY https://github.com/erincatto/box2d
    GIT_TAG v3.1.1
    GIT_SHALLOW ON)
    
FetchContent_MakeAvailable(BOX2D)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON)
    
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(TOML
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus
    GIT_TAG v3.4.0
    GIT_SHALLOW ON)

FetchContent_MakeAvailable(TOML)

# Lua: download the source and build it as a static library
FetchContent_Declare(lua
    GIT_REPOSITORY https://github.com/lua/lua
    GIT_TAG v5.4.7
    GIT_SHALLOW ON)
FetchContent_MakeAvailable(lua)

file(GLOB LUA_SOURCES ${lua_SOURCE_DIR}/*.c)
list(FILTER LUA_SOURCES EXCLUDE REGEX ".*/(lua|luac|onelua)\\.c$")   # these files have main() or include all files

add_library(lua_lib STATIC ${LUA_SOURCES})
target_include_directories(lua_lib PUBLIC ${lua_SOURCE_DIR})
set_target_properties(lua_lib PROPERTIES LINKER_LANGUAGE C)

FetchContent_Declare(sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2
    GIT_TAG v3.3.0
    GIT_SHALLOW ON
    SOURCE_SUBDIR does_not_exist)
FetchContent_MakeAvailable(sol2)

add_library(sol2_headers INTERFACE)
target_include_directories(sol2_headers SYSTEM INTERFACE ${sol2_SOURCE_DIR}/include)
target_link_libraries(sol2_headers INTERFACE lua_lib)

FetchContent_Declare(SPDLOG
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG v1.17.0
    GIT_SHALLOW ON)

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
    sol2_headers
)