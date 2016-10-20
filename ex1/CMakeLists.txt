cmake_minimum_required(VERSION 2.8)
project(cg1_ex1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ffast-math -march=native")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/")

set(EXTRA_LIBS "")

if(WIN32)
  list(APPEND EXTRA_LIBS opengl32)
  include_directories("${PROJECT_SOURCE_DIR}/win32/freeglut/include" "${PROJECT_SOURCE_DIR}/win32/glew-2.0.0/include")
  link_directories("${PROJECT_SOURCE_DIR}/win32/freeglut/lib" "${PROJECT_SOURCE_DIR}/win32/glew-2.0.0/lib/Release/Win32")
  list(APPEND EXTRA_LIBS freeglut glew32s)
elseif(APPLE)
  find_library(opengl_library OpenGL)
  list(APPEND EXTRA_LIBS ${opengl_library})
  include_directories("/opt/X11/include")
  link_directories("/opt/X11/lib")
  list(APPEND EXTRA_LIBS glut)
elseif(CMAKE_SYSTEM MATCHES "Linux")
  list(APPEND EXTRA_LIBS GL GLU glut)
endif()

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
set(SOURCE_FILES "${PROJECT_SOURCE_DIR}/Context.hpp" 
					"${PROJECT_SOURCE_DIR}/Context.cpp"
					"${PROJECT_SOURCE_DIR}/Control.hpp"
					"${PROJECT_SOURCE_DIR}/Control.cpp"
					"${PROJECT_SOURCE_DIR}/Debug.hpp"
					"${PROJECT_SOURCE_DIR}/glIncludes.hpp"
					"${PROJECT_SOURCE_DIR}/Node.hpp"
					"${PROJECT_SOURCE_DIR}/Node.cpp"
					"${PROJECT_SOURCE_DIR}/Robot.hpp"
					"${PROJECT_SOURCE_DIR}/Robot.cpp"
					"${PROJECT_SOURCE_DIR}/RotationMode.hpp"
					"${PROJECT_SOURCE_DIR}/SceneGraph.hpp"
					"${PROJECT_SOURCE_DIR}/SceneGraph.cpp"
)
					
add_executable(cg1_ex1 ${SOURCE_FILES})
target_link_libraries(cg1_ex1 ${EXTRA_LIBS})