cmake_minimum_required(VERSION 2.6)

project(senseless_soccer3)

# --------------------------------------------------
# environment variables
# --------------------------------------------------
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON )

# --------------------------------------------------
# include dirs
# --------------------------------------------------
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/sfml/include)


# --------------------------------------------------
# link dirs
# --------------------------------------------------
link_directories(${CMAKE_BINARY_DIR})

# --------------------------------------------------
# build files
# --------------------------------------------------
add_executable(senseless_soccer3
    src/main.cpp
    src/game/game.cpp
    src/game/gamestate.cpp
    src/menu/menu.cpp
    src/menu/widget.cpp
    src/menu/button.cpp
    src/menu/page.cpp
    src/input/gamepad.cpp
)
# --------------------------------------------------
# compiler flags
# --------------------------------------------------
if (WIN32)
    add_definitions(-std:c++latest)
endif (WIN32)

if (UNIX)
    add_definitions(-std=c++17)
    list(APPEND flags "-j8")
endif (UNIX)
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
# linker flags
# --------------------------------------------------
#SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
# --------------------------------------------------
# link libs
# --------------------------------------------------
find_package(OpenGL)
target_link_libraries (senseless_soccer3 -pthread)
target_link_libraries (senseless_soccer3 -lstdc++fs)
target_link_libraries (senseless_soccer3 ${OPENGL_gl_LIBRARY})

target_link_libraries (senseless_soccer3 sfml-system)
target_link_libraries (senseless_soccer3 sfml-window)
target_link_libraries (senseless_soccer3 sfml-graphics)


