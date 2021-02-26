#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const int WINDOW_WIDTH=1200;
const int WINDOW_HEIGHT=1200;
const char* WINDOW_TITLE="Antons OpenGL Tutorial";

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
  if( !glfwInit() ) abort( "Could not start GLFW3" );
}

void log_gl_version_info(){
  const std::string version( reinterpret_cast< char const* >( glGetString( GL_VERSION ) ) );
  log( "Renderer: " + std::string( reinterpret_cast< char const* >( glGetString( GL_RENDERER ) ) ) );
  log( "OpenGL Version support: " + version );
}

void process_input( GLFWwindow* window ) {
  // escape
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
  GLFWwindow* w = glfwCreateWindow( width, height, title, NULL, NULL );
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
    process_input( window );
    glfwSwapBuffers( window );
  }
}

int main() {
  init_glfw();

  GLFWwindow* window = init_window( WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE );
  init_glew();
  log_gl_version_info();
  init_gl();

  GLuint triangleVaoId = init_triangle_vao( triangleVerts );
  GLuint shaderProgramId = init_shader_program();

  render_loop( window, shaderProgramId, triangleVaoId );

  terminate_window();
}

