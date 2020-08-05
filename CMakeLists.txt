cmake_minimum_required(VERSION 3.18.0)
project(OpenGL_sort)
SET(SOURCE /home/darnoc/Documents/OpenGL/OpenGL/main.cpp /home/darnoc/Documents/OpenGL/OpenGL/Geometry/CodedMesh.cpp /home/darnoc/Documents/OpenGL/OpenGL/Sorts/MergeSort.h /home/darnoc/Documents/OpenGL/OpenGL/Geometry/CodedMesh.cpp /home/darnoc/Documents/OpenGL/OpenGL/Sorts/Quicksort.h /home/darnoc/Documents/OpenGL/OpenGL/Shaders/Functions/loadShader.cpp /home/darnoc/Documents/OpenGL/OpenGL/Sorts/Randomizer.h /home/darnoc/Documents/OpenGL/OpenGL/Input/Input.cpp /home/darnoc/Documents/OpenGL/OpenGL/Geometry/ShapeVertices.cpp /home/darnoc/Documents/OpenGL/OpenGL/Renderer/Renderer.cpp /home/darnoc/Documents/OpenGL/OpenGL/Camera.h) 
set(SOURCE_FILES /home/darnoc/Documents/OpenGL/main.cpp)
set(CMAKE_SOURCE_DIR /home/darnoc/Documents/OpenGL)

add_executable(${PROJECT_NAME} ${SOURCE})
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS} )

find_package(PkgConfig REQUIRED)
pkg_search_module(glfw3 REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GLFW_STATIC_LIBRARIES})

find_package(GLEW)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries (${PROJECT_NAME} ${GLEW_LIBRARIES})
endif (GLEW_FOUND)

find_package(glfw3)
if (glfw3_FOUND)
    include_directories(${glfw3_INCLUDE_DIRS})
    target_link_libraries (${PROJECT_NAME} ${glfw3_LIBRARIES})
endif (glfw3_FOUND)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set ( PROJECT_LINK_LIBS libGL.so.1 )
link_directories( /usr/lib/x86_64-linux-gnu/ )

set(PROJECT_SOURCE_DIR /home/darnoc/Documents/OpenGL)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY /home/darnoc/Documents/OpenGL/build/)
set(BASE_DIR /home/darnoc/Documents/OpenGL/)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(GLEW /home/darnoc/Documents/glew-2.2.0/include/GL/glew.h)
set(GLFW3 /home/darnoc/Documents/glfw-3.3.2/include/GLFW/glfw3.h)
set(GL /usr/include/GL/glew.h)
set(GL /usr/include/glm/glm.hpp)
set(GL /usr/include/glm/gtc/matrix_transform.hpp)
set(glew /home/darnoc/Documents/OpenGL/Dependencies/GLEW/include/glew.h)
set(glfw3 /home/darnoc/Documents/OpenGL/Dependencies/GLFW/include/glfw3.h)
set(GL /usr/lib/x86_64-linux-gnu/libglfw.so.3.2)
set(GL /usr/lib/x86_64-linux-gnu/libGL.so.1)
set(GL_LIBS -lX11 -lGL -lGLU -lGLEW -lGLUT)

link_directories("${PROJECT_SOURCE_DIR}")
link_directories(/home/darnoc/Documents/vcpkg/packages/glew_x64-linux/share/glew/vcpkg-cmake-wrapper.cmake)
link_directories(/home/darnoc/Documents/vcpkg)
link_directories(/usr/lib64)
link_directories(/usr/include)
link_directories(/usr/lib/x86_64-linux-gnu)
link_directories(/home/darnoc/Documents/glew-2.2.0)
link_directories(/home/darnoc/Documents/glfw-3.3.2) 
link_directories(/home/darnoc/Documents/freeglut-3.2.1)
link_directories(/home/darnoc/Documents/OpenGL/Dependencies/GLEW/include)
link_directories(/home/darnoc/Documents/OpenGL/Dependencies/GLFW/include)
link_directories(/usr/include/glm)
link_directories(/usr/include/X11)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories(/usr/lib64)
include_directories(/usr/include/GL)
include_directories(/usr/include/GLFW)
include_directories(/usr/include/glm)
include_directories(/usr/include/X11)

include_directories(/usr/lib)
include_directories(/usr/lib/x86_64-linux-gnu)
include_directories(/home/darnoc/Documents/OpenGL/Dependencies)
include_directories(/home/darnoc/Documents/glew-2.2.0)
include_directories(/home/darnoc/Documents/glfw-3.3.2) 
include_directories(/home/darnoc/Documents/freeglut-3.2.1)
include_directories(/home/darnoc/Documents/vcpkg)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
  libfl.a
  libGL.so.1
  libGLEW.a
  libGLEW.so
  libGLU.a
  libglut.a
  libl.a
  libglfw.so.3.2
  /home/darnoc/Documents/OpenGL/Dependencies/GLFW/lib/glfw3.lib
  /home/darnoc/Documents/OpenGL/Dependencies/GLEW/lib/Release/x64/glew32s.lib
)