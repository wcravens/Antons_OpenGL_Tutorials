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

GLuint init_triangle_vao( const GLfloat* data ); 
GLuint init_shader_program();

const int WINDOW_HEIGHT=1600;
const int WINDOW_WIDTH=1200;
const char* WINDOW_TITLE="Antons OpenGL Tutorial";

const GLfloat triangleVerts[] = {
   0.0f,  0.5f,  0.0f,
   0.5f, -0.5f,  0.0f,
  -0.5f, -0.5f,  0.0f
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

int main() {
  if( !glfwInit() ) abort( "Could not start GLFW3" );

  GLFWwindow* window;
  //initialize_window( window );
  #ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  #endif

  window = glfwCreateWindow( WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_TITLE, NULL, NULL );
  if( !window ) {
    glfwTerminate();
    abort( "Could not open window with GLFW3" );
  }
  glfwMakeContextCurrent( window );
  initialize_glew();

  log_gl_version_info();
  init_gl();

  GLuint triangleVaoId = init_triangle_vao( triangleVerts );
  GLuint shaderProgramId = init_shader_program();


  while( !glfwWindowShouldClose( window ) ){
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUseProgram( shaderProgramId );
    glBindVertexArray( triangleVaoId );
    glDrawArrays( GL_TRIANGLES, 0, 3 );
    glfwPollEvents();
    glfwSwapBuffers( window );
  }


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
