cmake_minimum_required(VERSION 2.6)

project(senseless_soccer3)

# --------------------------------------------------
# environment variables
# --------------------------------------------------
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON )

# --------------------------------------------------
# include dirs
# --------------------------------------------------
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# --------------------------------------------------
# lib dirs
# --------------------------------------------------
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib)

# --------------------------------------------------
# compiler flags
# --------------------------------------------------
if (WIN32)
    add_definitions(-std:c++latest)
endif (WIN32)

if (UNIX)
    add_definitions(-std=c++17)
endif (UNIX)

# --------------------------------------------------
# linker flags
# --------------------------------------------------
#SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

# --------------------------------------------------
# build files
# --------------------------------------------------
add_executable(senseless_soccer3
    src/main.cpp
    src/game/game.cpp
    src/game/gamestate.cpp
    src/game/menu.cpp
    src/input/gamepad.cpp
)
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(senseless_soccer3 PRIVATE
    -Werror
    -Wall
    -Wextra
    -Wno-unused-function
    -Wno-unused-parameter
    -Wno-implicit-fallthrough)
endif(CMAKE_COMPILER_IS_GNUCXX)

# --------------------------------------------------
# link libs
# --------------------------------------------------
find_package(OpenGL)
#target_link_libraries (senseless_soccer3 -fsanitize=address)
target_link_libraries (senseless_soccer3 -pthread)
target_link_libraries (senseless_soccer3 ${OPENGL_gl_LIBRARY})
target_link_libraries (senseless_soccer3 sfml-graphics)
target_link_libraries (senseless_soccer3 sfml-window)
target_link_libraries (senseless_soccer3 sfml-system)
