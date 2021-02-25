#include <iostream>

void log( const std::string message );
void log_error( const std::string message );
void quit( const std::string message );
void abort( const std::string message );

int main() {
  log( "Running" );
  //quit( "Exiting" );
  abort( "Failing" );
}

void log( const std::string message ) {
  std::cout << "LOG::" << message << std::endl;
}

void log_error( const std::string message ){
  std::cerr << "ERROR::" << message << std::endl;
}

void abort( const std::string message ){
  log_error( "ABORT::" + message );
  exit( EXIT_FAILURE );
};

void quit( const std::string message ){
  log( "QUIT::" + message );
  exit( EXIT_SUCCESS );
}
