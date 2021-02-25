# [Hello Triangle](https://antongerdelan.net/opengl/hellotriangle.html)

Define vertices in clockwise order.  This will allow us to determine the
'front' and 'back' of triangles later.

## Vertex Buffer Objects and Vertex Array Objects

Vertex Buffer Objects are used to store various information for vetices.
Model Points, Texture coordinates, vertex normals, vertex colors, etc.
Its common to use several of these for any given mesh.

We can use a Vertex Array Object (VAO) to store the Vertex Buffer
Objects (VBO).  The VAO stores references to all of the vertex buffers,
and the corresponding memory layout details for each one.

This way you can operate on a whole mesh at once rather than calling all
of the buffers you need one at a time, every time you need to do
something.  E.g. to draw, you just bind the VBO and draw.

## Shaders

We need shaders to tell OpenGL how we want to actually draw our
vertices.  At a minimum we need a Vertex Shader and a Fragment Shader.
Shaders are written in the OpenGL Shader Language (GLSL).

We use the Vertex Array Object Attribute Pointer to map data into the
shader.  Each attribute pointer has a location id to map data into the
shader.

The first vec3 input variable in our Vertex Shader is automatically
mapped to attribute `0` in our VAO.  And since the pointer has a size of
3 `GL_FLOAT`s, each vertex shader will get one point (set of three
coordinates) from the buffer.

These two lines pair up to achieve this;

    // In our c++ OpenGL application code
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );


    // In our GLSL vertex shader code  
    in vec3 vertexCoord;"


