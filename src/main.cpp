#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


void log( const std::string message );
void log_error( const std::string message );
void abort();
void abort( const std::string message );
void quit();
void quit( const std::string message );

void initialize_window( GLFWwindow* w);
void initialize_glew();
void log_gl_version_info();
void init_gl();
void terminate_window();

GLuint init_triangle_vbo( const float* data ); 

const int WINDOW_HEIGHT=1600;
const int WINDOW_WIDTH=1200;
const char* WINDOW_TITLE="Antons OpenGL Tutorial";

const float triangleVerts[] = {
   0.0f,  0.5f,  0.0f,
   0.5f, -0.5f,  0.0f,
  -0.5f, -0.5f,  0.0f
};

int main() {
  if( !glfwInit() ) abort( "Could not start GLFW3" );

  GLFWwindow* window;
  initialize_window( window );
  initialize_glew();
  log_gl_version_info();
  init_gl();

  GLuint triangleVboId = init_triangle_vbo( triangleVerts );
  terminate_window();

}

void log( const std::string message ) {
  std::cout << "LOG::" << message << "." << std::endl;
}

void log_error( const std::string message ){
  std::cerr << "ERROR::" << message << "!" << std::endl;
}

void abort(){
  exit( EXIT_FAILURE );
}

void abort( const std::string message ){
  log_error( "ABORT::" + message );
};

void quit(){
  exit( EXIT_SUCCESS );
};

void quit( const std::string message ){
  log( "QUIT::" + message );
}

void initialize_glew(){
  glewExperimental = GL_TRUE;
  glewInit();
  if (glewInit() != GLEW_OK)
      abort( "Failed to initialize GLEW");
}

void log_gl_version_info(){
  const std::string version( reinterpret_cast< char const* >( glGetString( GL_VERSION ) ) );
  log( "Renderer: " + std::string( reinterpret_cast< char const* >( glGetString( GL_RENDERER ) ) ) );
  log( "OpenGL Version support: " + version );
}

void init_gl(){
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LESS );
}

void initialize_window( GLFWwindow* w ){
  #ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  #endif

  w = glfwCreateWindow( WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_TITLE, NULL, NULL );
  if( !w ) {
    glfwTerminate();
    abort( "Could not open window with GLFW3" );
  }
  glfwMakeContextCurrent( w );
}

void terminate_window(){
  glfwTerminate();
  quit();
}

GLuint init_triangle_vbo( const float* data ) {
  GLuint vboId;
  glGenBuffers(1, &vboId );
  glBindBuffer( GL_ARRAY_BUFFER, vboId );
  glBufferData( GL_ARRAY_BUFFER, sizeof( data ), data, GL_STATIC_DRAW );

  GLuint vaoId;
  glGenVertexArrays( 1, &vaoId );
  glBindVertexArray( vaoId );
  glEnableVertexAttribArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, vboId );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );

  return vboId;
}

