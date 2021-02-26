#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <time.h>
#include <stdarg.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GL_LOG_FILE "ogltut_gl.log"

// TODO: Convert to #define's
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1200
#define WINDOW_TITLE "Antons OpenGL Tutorial"

const GLfloat triangleVerts[] = {
   0.25f,  0.75/2.0,  0.0f, // top
   0.5f,  0.0f,  0.0f, // lower right
   0.0f,  0.0f,  0.0f  // lower left
};

const char* vertex_shader =
"#version 400\n"
"in vec3 vertexCoord;"
"void main() {"
"  gl_Position = vec4(vertexCoord, 1.0);"
"}";

const char* fragment_shader =
"#version 400\n"
"out vec4 frag_colour;"
"void main() {"
"  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
"}";

int g_gl_width = WINDOW_WIDTH;
int g_gl_height = WINDOW_HEIGHT;

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

bool restart_gl_log(){
  std::ofstream ofs( GL_LOG_FILE, std::ofstream::out | std::ofstream::trunc );
  if( ! ofs.is_open() ){
    std::cerr << "ERROR: could not open GL_LOG_FILE for writing: " << GL_LOG_FILE << std::endl;
    return false;
  }

  // We may also be able to use __DATE__ and __TIME__ with `gcc`.
  time_t now = time( NULL );
  char* date = ctime( &now );
  ofs << "INFO::GL_LOG_FILE::RESTART::"<< date;
  ofs.close();
  return true;
}

bool gl_log( const char* message ){
  std::ofstream ofs( GL_LOG_FILE, std::ofstream::out | std::ofstream::app );
  if( ! ofs.is_open() ){
    std::cerr << "ERROR: could not open GL_LOG_FILE for append: " << GL_LOG_FILE << std::endl;
    return false;
  }
  ofs << "INFO::" << message << std::endl;
  ofs.close();
  return true;
}

bool gl_log_error( const char* message ){
  std::ofstream ofs( GL_LOG_FILE, std::ofstream::out | std::ofstream::app );
  if( ! ofs.is_open() ){
    std::cerr << "ERROR: could not open GL_LOG_FILE for append: " << GL_LOG_FILE << std::endl;
    return false;
  }
  std::cerr << "ERROR::" << message << std::endl;
  ofs << "ERROR::" << message << std::endl;
  ofs.close();
  return true;
}

void glfw_error_callback( int error, const char* description ){
  std::string msg( "GLFW::" + std::to_string( error ) + "::" + description );
  gl_log_error( msg.c_str() ); 
}

void glfw_window_size_callback( GLFWwindow* window, int width, int height ) {
  g_gl_width = width;
  g_gl_height = height;
  /* Update perspective matrices here */
}

void init_gl(){
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LESS );
}

void init_glew(){
  glewExperimental = GL_TRUE;
  glewInit();
  if (glewInit() != GLEW_OK)
      abort( "Failed to initialize GLEW");
}

void init_glfw() {
  std::string msg( "GLFW::STARTING::" );
  msg.append( glfwGetVersionString() );
  gl_log( msg.c_str() );
  glfwSetErrorCallback( glfw_error_callback );
  if( !glfwInit() ){
    const char* msg = "Could not start glfw.";
    gl_log_error( msg );
    abort( "Could not start GLFW3" );
  }
}

void log_gl_version_info(){
  const std::string version( reinterpret_cast< char const* >( glGetString( GL_VERSION ) ) );
  log( "Renderer: " + std::string( reinterpret_cast< char const* >( glGetString( GL_RENDERER ) ) ) );
  log( "OpenGL Version support: " + version );
}

void process_input( GLFWwindow* window ) {
  // List of all the keycodes and input handling commands: http://www.glfw.org/docs/latest/group__input.html 
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}


void terminate_window(){
  glfwTerminate();
  quit();
}

GLuint init_shader_program(){
  GLuint vertexShaderId = glCreateShader( GL_VERTEX_SHADER );
  glShaderSource( vertexShaderId, 1, &vertex_shader, NULL );
  glCompileShader( vertexShaderId );

  GLuint fragmentShaderId = glCreateShader( GL_FRAGMENT_SHADER );
  glShaderSource( fragmentShaderId, 1, &fragment_shader, NULL );
  glCompileShader( fragmentShaderId );

  GLuint shaderProgramId = glCreateProgram();
  glAttachShader( shaderProgramId, vertexShaderId );
  glAttachShader( shaderProgramId, fragmentShaderId );
  glLinkProgram( shaderProgramId );

  return shaderProgramId;
}

GLuint init_triangle_vao( const GLfloat* data ) {
  GLuint vboId;
  glGenBuffers(1, &vboId );
  glBindBuffer( GL_ARRAY_BUFFER, vboId );
  glBufferData( GL_ARRAY_BUFFER, 9 * sizeof( GLfloat ), data, GL_STATIC_DRAW );

  GLuint vaoId;
  glGenVertexArrays( 1, &vaoId );
  glBindVertexArray( vaoId );
  glEnableVertexAttribArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, vboId );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );

  return vboId;
}

GLFWwindow* init_window( int width, int height, const char* title ){
  #ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  #endif
  glfwWindowHint( GLFW_SAMPLES, 4 );

  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* videoMode = glfwGetVideoMode( monitor );
  std::stringstream ss;
  ss << "width:" << videoMode->width << ",height:" << videoMode->height << 
                ",redBits:" << videoMode->redBits << ",blueBits:" << videoMode->blueBits << ",greenBits:" << videoMode->greenBits <<
                ",refreshRate:" << videoMode->refreshRate;
  g_gl_width = videoMode->width;
  g_gl_height = videoMode->height;
  GLFWwindow* w = glfwCreateWindow( g_gl_width, g_gl_height, title, monitor, NULL );
  glfwSetWindowSizeCallback( w, glfw_window_size_callback );
  gl_log( ss.str().c_str() );

  if( !w ) {
    glfwTerminate();
    abort( "Could not open window with GLFW3" );
  }
  glfwMakeContextCurrent( w );
  return w;
}

void render_loop( GLFWwindow* window, GLuint shaderId, GLuint vaoId ) {
  while( !glfwWindowShouldClose( window ) ){
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUseProgram( shaderId );
    glBindVertexArray( vaoId );
    glDrawArrays( GL_TRIANGLES, 0, 3 );
    glfwPollEvents();
    glfwSwapBuffers( window );
    process_input( window );
  }
}

int main() {
  assert( restart_gl_log() );
  init_glfw();

  GLFWwindow* window = init_window( g_gl_width, g_gl_height, WINDOW_TITLE );
  init_glew();
  log_gl_version_info();
  init_gl();

  GLuint triangleVaoId = init_triangle_vao( triangleVerts );
  GLuint shaderProgramId = init_shader_program();

  render_loop( window, shaderProgramId, triangleVaoId );

  terminate_window();
}

