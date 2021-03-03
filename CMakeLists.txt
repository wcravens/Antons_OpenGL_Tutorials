cmake_minimum_required(VERSION 3.16)
project(AntonsOpenGlTutorials)
set_property(GLOBAL PROPERTY GLOBAL_DEPENDS_DEBUG_MODE 1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors")

set(CMAKE_CXX_STANDARD 17)

add_executable( ogltut src/main.cpp )

message( ${CMAKE_CURRENT_BINARY_DIR} )
message( ${CMAKE_CURRENT_SOURCE_DIR} )

# https://stackoverflow.com/questions/36483801/cmake-softlink-resource-such-as-glsl-shaders-or-copy-each-complilation
ADD_CUSTOM_TARGET(
    copy_shader_files
    ${CMAKE_COMMAND}
    -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders
    ${CMAKE_CURRENT_BINARY_DIR}/shaders
    COMMENT "Copying shader files." VERBATIM 
)
 
add_dependencies( ogltut copy_shader_files )

include_directories( "include" )

find_package( GLEW REQUIRED )
find_package( glfw3 3.3 REQUIRED)
find_package( OpenGL REQUIRED )
#find_package( glm REQUIRED )

target_link_libraries( ogltut GLEW glfw OpenGL::GL ${CMAKE_DL_LIBS} )
