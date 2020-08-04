cmake_minimum_required(VERSION 3.0.0)
project(OpenGL VERSION 0.1.0)
SET(SOURCE /home/darnoc/Documents/OpenGL/OpenGL/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY /home/darnoc/Documents/OpenGL/build/)
set(BASE_DIR /home/darnoc/Documents/OpenGL/)
set(SOURCE_FILES /home/darnoc/Documents/OpenGL/)
set(GLEW /home/darnoc/Documents/glew-2.2.0/include/GL/glew.h)
set(GLFW3 /home/darnoc/Documents/glfw-3.3.2/include/GLFW/glfw3.h)
set(GL /usr/include/GL/glew.h)
set(GL /usr/include/glm/glm.hpp)
set(GL /usr/include/glm/gtc/matrix_transform.hpp)
set(glew /home/darnoc/Documents/OpenGL/Dependencies/GLEW/include/glew.h)
set(glfw3 /home/darnoc/Documents/OpenGL/Dependencies/GLFW/include/glfw3.h)

link_directories(/home/darnoc/Documents/vcpkg/packages/glew_x64-linux/share/glew/vcpkg-cmake-wrapper.cmake /usr/include /usr/lib/x86_64-linux-gnu /home/darnoc/Documents/OpenGL/Dependencies /home/darnoc/Documents/glew-2.2.0 /home/darnoc/Documents/glfw-3.3.2 /home/darnoc/Documents/freeglut-3.2.1)

find_library(GLEW glew HINTS /home/darnoc/Documents/glew-2.2.0/include/GL/glew.h)
target_link_libraries(OpenGL PUBLIC ${GLEW})

find_library(GLFW3 glfw3 HINTS /home/darnoc/Documents/glfw-3.3.2/include/GLFW/glfw3.h)
target_link_libraries(OpenGL PUBLIC ${GLFW3})

find_package(GLEW REQUIRED)
target_link_libraries(OpenGL PRIVATE GLEW::GLEW)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(OpenGL PRIVATE glfw)



