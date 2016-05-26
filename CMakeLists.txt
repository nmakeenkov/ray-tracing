# TODO: change, as it's an old version, not using cmake in new ones

cmake_minimum_required(VERSION 3.3)
project(graphic_library)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################
# Include Files
#########################################################
add_executable(graphic_library main.cpp MyGL/Color.cpp
        Geometry3d/Vector.cpp Geometry3d/Ray.cpp Geometry3d/Sphere.cpp
        Geometry3d/Parallelogram.cpp Geometry3d/Intersection.cpp
        Geometry3d/Plane.cpp Geometry3d/Utils.cpp MyGL/Readers/ReadSTL.cpp
        MyGL/Scene.cpp MyGL/Examples/Examples.hpp Geometry3d/Triangle.cpp)

########################################################
# Linking & stuff
#########################################################
target_link_libraries(graphic_library -lglut -lGL -lGLU)