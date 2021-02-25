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
void terminate_window();

const int WINDOW_HEIGHT=1600;
const int WINDOW_WIDTH=1200;
const char* WINDOW_TITLE="Antons OpenGL Tutorial";

int main() {
  if( !glfwInit() ) abort( "Could not start GLFW3" );

  GLFWwindow* window;
  initialize_window( window );
  initialize_glew();
  log_gl_version_info();

  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LESS );

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

