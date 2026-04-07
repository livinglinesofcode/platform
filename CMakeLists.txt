cmake_minimum_required(VERSION 3.16)
project(MyEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_library(GLESv2_LIB NAMES GLESv2 libGLESv2.so)

# Angelscript
file(GLOB ANGELSCRIPT_SOURCES
	engine/scripting/angelscript/source/*.cpp
	engine/scripting/angelscript/add_on/*/*.cpp
)

add_library(angelscript STATIC ${ANGELSCRIPT_SOURCES})
target_include_directories(angelscript PUBLIC
	engine/scripting/angelscript/include
	engine/scripting/angelscript/add_on
)

# Shaders
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/engine/rendering/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# glad
add_library(glad STATIC engine/rendering/glad/src/glad.c)
target_include_directories(glad PUBLIC engine/rendering/glad/include)

# Engine
file(GLOB ENGINE_SOURCES
	engine/math/*.cpp
	engine/scripting/script_bindings.cpp
)

add_library(engine STATIC ${ENGINE_SOURCES})
target_include_directories(engine PUBLIC
	engine
	engine/scripting/angelscript/include
	engine/scripting/angelscript/add_on
	engine/rendering
)

target_compile_options(engine PRIVATE
	-Wall
	-Wextra
	-Wpedantic
	-Werror
	-Wshadow
	-Wconversion
)

target_link_libraries(engine PUBLIC angelscript glad)

# Editor
add_executable(editor editor/src/main.cpp)
target_link_libraries(editor PRIVATE engine ${SDL2_LIBRARIES} ${GLESv2_LIB})

# Client
#add_executable(client client/launcher/main.cpp)
#target_link_libraries(client PRIVATE engine ${SDL2_LIBRARIES} ${GLESv2_LIB})
