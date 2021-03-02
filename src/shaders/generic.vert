#version 420

in vec3 vertexCoord;

void main() {
  gl_Position = vec4(vertexCoord, 1.0);
}

