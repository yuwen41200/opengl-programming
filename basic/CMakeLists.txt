# Install the dependencies:
#   $ sudo apt-get install freeglut3-dev libxmu-dev libxi-dev
# Compile the programs:
#   $ g++ -std=c++11 -Wall -Wextra -Wpedantic source.cpp -lGL -lGLU -lglut

cmake_minimum_required(VERSION 3.6)
project(opengl_programming)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wpedantic")
set(SOURCE_FILES basic1.cpp)
add_executable(opengl_programming ${SOURCE_FILES})
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(opengl_programming ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
