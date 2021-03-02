#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <time.h>
#include <stdarg.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GL_LOG_FILE "ogltut_gl.log"

#define WINDOW_WIDTH        1200
#define WINDOW_HEIGHT       1200
#define WINDOW_TITLE        "Antons OpenGL Tutorial"
#define WINDOW_FULL_SCREEN  0

#define GENERIC_VERTEX_SHADER_FILE "shaders/generic.vert"
#define GENERIC_FRAGMENT_SHADER_FILE "shaders/generic.frag"

const GLfloat triangleVerts[] = {
   0.25f,  0.75/2.0,  0.0f, // top
   0.5f,  0.0f,  0.0f, // lower right
   0.0f,  0.0f,  0.0f  // lower left
};

int g_gl_width  = WINDOW_WIDTH;
int g_gl_height = WINDOW_HEIGHT;

bool _gl_log_to_file( std::string msg ){
  std::ofstream ofs( GL_LOG_FILE, std::ofstream::out | std::ofstream::app );
  if( ! ofs.is_open() ){
    std::cerr << "ERROR: could not open GL_LOG_FILE for append: " << GL_LOG_FILE << std::endl;
    return false;
  }
  ofs <<  msg << std::endl;
  ofs.close();
  return true;
}

bool gl_log( std::string msg ){
  std::string prefix( "INFO::" );
  return _gl_log_to_file( prefix + msg );
}

bool gl_log_error( std::string msg ){
  std::string prefix( "ERROR::" );
  std::cerr << prefix+msg << std::endl;
  return _gl_log_to_file( prefix + msg );
}

bool gl_log_restart(){
  std::ofstream ofs( GL_LOG_FILE, std::ofstream::trunc );
  if( !ofs.is_open() ){
    std::cerr << "ERROR:: Could not start new log file " << GL_LOG_FILE << "!";
    return false;
  }
  // We may also be able to use __DATE__ and __TIME__ with `gcc` to report build version.
  time_t now = time( NULL );
  std::string date = ctime( &now );
  ofs <<  "*****RESTART***** " << date;
  ofs.close();
  return true;

}

void gl_log_params() {
  GLenum params[] = {
    GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
    GL_MAX_CUBE_MAP_TEXTURE_SIZE,
    GL_MAX_DRAW_BUFFERS,
    GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
    GL_MAX_TEXTURE_IMAGE_UNITS,
    GL_MAX_TEXTURE_SIZE,
    GL_MAX_VARYING_FLOATS,
    GL_MAX_VERTEX_ATTRIBS,
    GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
    GL_MAX_VERTEX_UNIFORM_COMPONENTS,
    GL_MAX_VIEWPORT_DIMS,
    GL_STEREO,
  };
  const char* names[] = {
    "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
    "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
    "GL_MAX_DRAW_BUFFERS",
    "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
    "GL_MAX_TEXTURE_IMAGE_UNITS",
    "GL_MAX_TEXTURE_SIZE",
    "GL_MAX_VARYING_FLOATS",
    "GL_MAX_VERTEX_ATTRIBS",
    "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
    "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
    "GL_MAX_VIEWPORT_DIMS",
    "GL_STEREO",
  };

  gl_log("GL Context Params:");
  gl_log("-----------------------------");

  // integers - only works if params 0-9 are integer return types
  for (int i = 0; i <= 9; i++) {
    int v = 0;
    glGetIntegerv(params[i], &v);

    std::string msg;
    msg += names[i] + std::string(": ") + std::to_string( v ) ;
    gl_log( msg );
  }
  // others
  int v[2] = {0};
  glGetIntegerv(params[10], v);
  std::string msg;
  msg += names[10] + std::string(":") + std::to_string( v[0] ) + "," + std::to_string( v[1] );
  gl_log( msg );

  unsigned char s = 0;
  glGetBooleanv(params[11], &s);
  msg = "";
  msg += names[11] + std::string(":") + std::to_string( s );
  gl_log( msg );
  gl_log("-----------------------------");
}

void abort(){
  gl_log_error( "ABORT!!!" );
  exit( EXIT_FAILURE );
};

void quit(){
  exit( EXIT_SUCCESS );
};

void quit( const std::string message ){
  gl_log( "QUIT::" + message );
}

void glfw_error_callback( int error, const char* description ){
  std::string msg( "GLFW::" + std::to_string( error ) + "::" + description );
  gl_log_error( msg.c_str() );
}

void glfw_window_size_callback( GLFWwindow* window, int width, int height ) {
  g_gl_width  = width;
  g_gl_height = height;
  glViewport( 0, 0, g_gl_width, g_gl_height );
  /* Update perspective matrices here */
}

void init_gl(){
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LESS );
}

void init_glew(){
  glewExperimental = GL_TRUE;
  glewInit();
  if (glewInit() != GLEW_OK) {
      gl_log_error( "INIT_GLEW::Failed to initialize GLEW!" );
      abort();
  }
}

void init_glfw() {
  gl_log( std::string("IT_GLFW::Starting ") + glfwGetVersionString() );

  glfwSetErrorCallback( glfw_error_callback );
  if( !glfwInit() ){
    gl_log_error( "INIT_GLFW::Could not start glfw!" );
    abort();
  }
}

void log_gl_version_info(){
  const std::string renderer( reinterpret_cast< char const* >( glGetString( GL_RENDERER ) ) );
  gl_log( std::string("Renderer: ")               + renderer ); 

  const std::string version( reinterpret_cast< char const* >( glGetString( GL_VERSION ) ) );
  gl_log( std::string("OpenGL Version support: ") + version ); 
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

std::string read_text_from_file( const char* filepath ) {
  gl_log( "Reading file " + std::string( filepath ) );
  std::ifstream ifs( filepath, std::ifstream::binary );
  if( !ifs ){
    gl_log_error( std::string( "READ_FILE::Failed to open ") + filepath + std::string("!") );
    abort();
  }
  std::ostringstream sstr;
  sstr << ifs.rdbuf();
  return sstr.str();
}

GLuint init_shader_program(){

  const char* vertex_shader_code    = read_text_from_file( GENERIC_VERTEX_SHADER_FILE ); 
  const char* fragment_shader_code  = read_text_from_file( GENERIC_FRAGMENT_SHADER_FILE ); 

  GLuint vertexShaderId = glCreateShader( GL_VERTEX_SHADER );
  glShaderSource( vertexShaderId, 1, &vertex_shader_code, NULL );
  glCompileShader( vertexShaderId );

  GLuint fragmentShaderId = glCreateShader( GL_FRAGMENT_SHADER );
  glShaderSource( fragmentShaderId, 1, &fragment_shader_code, NULL );
  glCompileShader( fragmentShaderId );

  GLuint shaderProgramId = glCreateProgram();
  glAttachShader( shaderProgramId, vertexShaderId );
  glAttachShader( shaderProgramId, fragmentShaderId );
  glLinkProgram( shaderProgramId );

  int params = -1;
  glGetProgramiv( shaderProgramId, GL_ACTIVE_UNIFORMS, &params );
  gl_log( std::to_string( params ) );

  GLint uniformLoc = glGetUniformLocation( shaderProgramId, "inputColor" );
  gl_log( "UNIFORM_LOCATION::inputColor::" + uniformLoc );
  glUseProgram( shaderProgramId );
  glUniform4f( uniformLoc, 0.5, 0.0, 0.5, 1.0 );

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

GLFWwindow* init_window(){
  std::stringstream ss;

  #ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  #endif
  glfwWindowHint( GLFW_SAMPLES, 4 );

  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* videoMode = glfwGetVideoMode( monitor );
  ss << "VIDEO_MODE::width:" << videoMode->width << ",height:" << videoMode->height << 
                ",redBits:" << videoMode->redBits << ",blueBits:" << videoMode->blueBits << ",greenBits:" << videoMode->greenBits <<
                ",refreshRate:" << videoMode->refreshRate;
  gl_log( ss.str().c_str() );
  ss.clear();
  ss.str("");

  if( WINDOW_FULL_SCREEN ){
    g_gl_width  = videoMode->width;
    g_gl_height = videoMode->height;
    char tmp[128];
    sprintf( tmp, "INIT_WINDOW::Full screen window. width: %d, height: %d.", g_gl_width, g_gl_height );
    gl_log( tmp );
  }
  char tmp[128];
  sprintf( tmp, "INIT_WINDOW::width: %d, height: %d, title: %s.", g_gl_width, g_gl_height, WINDOW_TITLE );
  gl_log( tmp ); 

  GLFWwindow* w = glfwCreateWindow( g_gl_width, g_gl_height, WINDOW_TITLE, NULL, NULL );
  if( !w ) {
    glfwTerminate();
    gl_log_error( "INIT_WINDOW::Could not open window with GLFW3!" );
    abort();
  }
  glfwMakeContextCurrent( w );
  glfwSetWindowSizeCallback( w, glfw_window_size_callback );
  gl_log_params();
  return w;
}

#define WINDOW_TITLE_FPS_DEBOUNCE 0.25
void _update_fps_counter( GLFWwindow* window ){
  static double previous_seconds = glfwGetTime();
  static int frame_count;
  double current_seconds = glfwGetTime();
  double elapsed_seconds = current_seconds - previous_seconds;
  if( elapsed_seconds > WINDOW_TITLE_FPS_DEBOUNCE ) {
    previous_seconds = current_seconds;
    double fps = (double)frame_count / elapsed_seconds;
    char tmp[128];
    sprintf( tmp, "WINDOW_TITLE : drawing @ %.2f fps", fps );
    glfwSetWindowTitle( window, tmp );
    frame_count = 0;
  }
  frame_count++;
}

void render_loop( GLFWwindow* window, GLuint gpuProgramId, GLuint vaoId ) {
  while( !glfwWindowShouldClose( window ) ){
    _update_fps_counter( window );
    // New buffer state
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Drawing
    glUseProgram( gpuProgramId );
    glBindVertexArray( vaoId );
    glDrawArrays( GL_TRIANGLES, 0, 3 );

    // Buffer
    glfwSwapBuffers( window );

    // Intermittent Events
    glfwPollEvents();
    process_input( window );
  }
}

GLFWwindow* setup() {
  init_glfw();
  GLFWwindow* w = init_window();
  init_glew();
  init_gl();
  log_gl_version_info();
  return w;
}

int main() {
  assert( gl_log_restart() );

  GLFWwindow* window = setup();

  GLuint triangleVaoId = init_triangle_vao( triangleVerts );
  GLuint shaderProgramId = init_shader_program();

  glViewport( 0, 0, g_gl_width, g_gl_height );

  render_loop( window, shaderProgramId, triangleVaoId );

  terminate_window();
}

