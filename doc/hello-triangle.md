# [Hello Triangle](https://antongerdelan.net/opengl/hellotriangle.html)

Define vertices in clockwise order.  This will allow us to determine the
'front' and 'back' of triangles later.

Vertex Buffer Objects are used to store various information for vetices.
Model Points, Texture coordinates, vertex normals, vertex colors, etc.
Its common to use several of these for any given mesh.

We can use a Vertex Array Object (VAO) to store the Vertex Buffer
Objects (VBO).  The VAO stores references to all of the vertex buffers,
and the corresponding memory layout details for each one.

This way you can operate on a whole mesh at once rather than calling all
of the buffers you need one at a time, every time you need to do
something.  E.g. to draw, you just bind the VBO and draw.
