include(FetchContent)

cmake_minimum_required(VERSION 3.20.0)
project(HelloSDL VERSION 0.1.0 LANGUAGES C CXX)

# Debug
# set(CMAKE_VERBOSE_MAKEFILE ON)

# C++ Version stuff
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Dependencies
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)

# Create an option to switch between a system sdl library and a vendored sdl library
option(MYGAME_VENDORED "Use vendored libraries" ON)

if(MYGAME_VENDORED)
    add_subdirectory(vendored/SDL2 EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
    find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

    # 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
endif()

#glew, OPENGL
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

#add_subdirectory(vendored/glew/build/cmake EXCLUDE_FROM_ALL)

# Create your game executable target as usual
# Apparently this is bad practice, but I'm lazy to manually add every single new source file
# file(GLOB_RECURSE SourceFiles src/**.cpp)
add_executable(HelloSDL src/main.cpp
        src/engine/engine.cpp
        src/engine/gl/gl_shader.cpp
        src/engine/util/ExpectedWrapper.cpp
        src/game.cpp
    )
target_include_directories(HelloSDL PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_property(TARGET HelloSDL PROPERTY CXX_STANDARD 23)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(HelloSDL PRIVATE SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(HelloSDL PRIVATE SDL2::SDL2)

# Link other dependencies
target_link_libraries(HelloSDL PRIVATE fmt::fmt OpenGL::GL GLEW::GLEW)

#copy shaders
# add_custom_command(
#         TARGET HelloSDL POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#                 ${CMAKE_SOURCE_DIR}/src/shaders
#                 ${CMAKE_CURRENT_BINARY_DIR}/shaders)