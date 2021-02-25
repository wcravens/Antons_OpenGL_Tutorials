cmake_minimum_required(VERSION 3.16)
project(AntonsOpenGlTutorials)
set_property(GLOBAL PROPERTY GLOBAL_DEPENDS_DEBUG_MODE 1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors")

set(CMAKE_CXX_STANDARD 17)

add_executable( ogltut src/main.cpp )

include_directories( "include" )

#find_package( glfw3 3.3 REQUIRED)
find_package( OpenGL REQUIRED )
#find_package( glm REQUIRED )

#target_link_libraries( learngl glfw glm::glm OpenGL::GL ${CMAKE_DL_LIBS} )
target_link_libraries( ogltut OpenGL::GL ${CMAKE_DL_LIBS} )
