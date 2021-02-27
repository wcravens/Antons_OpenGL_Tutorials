cmake_minimum_required(VERSION 3.16)
project(AntonsOpenGlTutorials)
set_property(GLOBAL PROPERTY GLOBAL_DEPENDS_DEBUG_MODE 1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors")

set(CMAKE_CXX_STANDARD 17)

file( GLOB GLSL_VERTEX_SHADER_FILES "src/*.vert" )
file( GLOB GLSL_FRAGMENT_SHADER_FILES "src/*.frag" )

file( COPY ${GLSL_VERTEX_SHADER_FILES} DESTINATION shaders )
message( "Copying ${GLSL_VERTEX_SHADER_FILES}" )
file( COPY ${GLSL_FRAGMENT_SHADER_FILES} DESTINATION shaders )
message( "Copying ${GLSL_FRAGMENT_SHADER_FILES}" )

add_executable( ogltut src/main.cpp )

include_directories( "include" )

find_package( GLEW REQUIRED )
find_package( glfw3 3.3 REQUIRED)
find_package( OpenGL REQUIRED )
#find_package( glm REQUIRED )

target_link_libraries( ogltut GLEW glfw OpenGL::GL ${CMAKE_DL_LIBS} )
