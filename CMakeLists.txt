cmake_minimum_required(VERSION 3.16)
project(MyEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_library(GLESv2_LIB NAMES GLESv2 libGLESv2.so)

# Engine
file(GLOB ENGINE_SOURCES 
    engine/math/*.cpp
    engine/scripting/angelscript/source/*.cpp
    engine/scripting/script_bindings.cpp
)

file(GLOB ADDON_SOURCES engine/scripting/angelscript/add_on/*/*.cpp)
list(APPEND ENGINE_SOURCES ${ADDON_SOURCES})

add_library(engine STATIC ${ENGINE_SOURCES})

target_include_directories(engine PUBLIC 
    engine
    engine/scripting/angelscript/include
    engine/scripting/angelscript/add_on
)

# Editor
add_executable(editor editor/src/main.cpp)
target_link_libraries(editor PRIVATE engine ${SDL2_LIBRARIES} ${GLESv2_LIB})

# Client
add_executable(client client/launcher/main.cpp)
target_link_libraries(client PRIVATE engine ${SDL2_LIBRARIES} ${GLESv2_LIB})
