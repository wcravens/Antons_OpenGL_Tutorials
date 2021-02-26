#[OpenGL 4 Shaders](file:///home/wcravens/Repos/OpenGL/Antons_OpenGL_Tutorials_Website/shaders.html)

The rendering process has several distinct stages of transforming a 3d
object in a final 2d image.  We call this staged process the **graphics
pipeline**.  All of the stages of the pipeline that happen on the GPI
are called the **[programable] hardware pipeline**.

## OpenGL 4 Hardware Pipeline

In OpenGL 4 we can write shaders to control many stages of the hardware
pipeline.  Think of operations such as `glDrawArrays` as a signal to the
*hardware pipeline* that it should begin the operations according to the
state that you have already configured in the *hardware pipeline*.

Several stages that can have custom shaders, some do not...

  - **Vertex Shader**
  - **Tesselation Control Shader**
  - *Tesselator*
  - **Tesselation Evaluation Shader**
  - **Geometry Shader**
  - *Clipping*
  - *Rasterization*
  - **Fragment Shader**
  - *Blending*

[*See pipeline image in main text*]

Shaders are individually compiled during runtim and then linked together
into a *Program*. You can think of this program as the *hardware
pipeline* for one of the parallel channels in the rendering process.
Depending on the number of GPUs available, and the number of vertices in
a mesh; all verticies in a mesh may be computed simultaneously in
parallel.

It's worth remembering that we only *draw* one mesh at a time.  Thus
keeping the number of meshes drawn per-scene can be beneficial.

## Difference between fragments and pixels.

Pixels are individual elements of the final 2D image that we draw to our
screen.  Fragments are pixel sized areas of a virtual surface in our
projection.  Fragment Shaders determine the final color of every
fragment.  There could be several of them overlapping each other for
any given picture.  All of these fragements are drawn - even the hidden
ones.

All fragements are drawn and written into the framebuffer image that
will be displayed as the final pixels.  If depth testing is enabled then
fragments that are *on top of* other fragments will overwrite them in
the buffers. Some GPUs are smart enough to catch this and skip drawing
*hidden* fragments.  But this is difficult in practice, and so we will
end up executing huge numbers of redundant fragment shaders.

## Shader Language

The OpenGL Shader Language (GLSL) is used to write shaders. Version 3.2
added Geometry Shaders, and 4.0 added Tessellation.  Version tag 400, or
later should work for our purposes.

    #version 420 // Because that's just funny.

## GLSL Operators

GLSL contains the C and C++ operators with the exception of pointers.

## GLSL Data Types

[Data Types - Official Reference Documantation](http://www.opengl.org/documentation/glsl/)

Commonly Used Data Types: `void`, `bool`, `int`, `float`, `vec[3,4]`,
`mat[3,4]`, `sampler2D`, `samplerCube`, and `sampler2DShadow`.

## File naming convention

The GLSL reference compiler
[Glslang](http://www.khronos.org/opengles/sdk/tools/Reference-Compiler/)
can check for bugs if the files are named `*.vert`, and `*.frag`.
Consider using this naming convention from the tutorial author:

texturemap.vert
texturemap.frag
partical.vert
partical.geom
partical.frag

## Example Shaders

Each shader is a small c-like GLSL program. Shader definitions can be
stored as literals in a C string, or loaded from a plain text file.  But
they are always compiled during runtime according to GPU vender
specifications.

In the simplest case we need a vertex shader to recieve data into the
hardware pipeline, and a fragement shader to determine color.  The
vertex shader will simply accept a buffer of point and place them
directly into the scene.  We will control the color from outside the
hardware pipeline by useing a `uniform` variable that is a global
available to all shaders in the program, but can be set after the
program is built and linked.

## Vertex Shader

The simplest vertex shader simply accepts points from a buffer and
passes them unaltered to the downstream pipeline.

`basic.vert`

    #version 420
    in vec3 vertex_position;
    void main(){
      gl_Position = vec4( vertex_position, 1.0 );
    }

`vec3` is a 3d vector that can be used to store position, directions, or
colors.

`vec4` is the same but has a fourth component which, in this variable,
is used to determine perspective. `1.0` means don't calculate
perspective.

Every shader has an entry point of `void main()`.

`gl_Position` is a built-in GLSL variable used to set the final
clip-space position of each vertex.

The input to the Vertex Shader is defined via the `in` keywork.  In this
case `vertex_position` is one of the vertex points from the object
vertex buffer that we are currently drawing.  GLSL also has an `out`
keywork to define variables that are sent down the pipeline.

The `in`put variables are call per-vertex **attributes**, and come from
blocks of memory on the GPU call **vertex buffers**.  We usually load
the vertex buffers before running our main loop.  The vertex shader, and
the downstreap pipeline shaders are all ran once per vertex. In other
words, one vertex is processed at a time through the pipeline.

## Fragment Shader

Once all vertex shaders have been computed for every vertex inside the
clip-space, a fragment shader is ran once for every pixel-sized space
(fragement) between the vertices.

`basic.frag`

    #version 420

    uniform vec4 inputColor;
    out     vec4 fragColor;

    void main(){
      fragColor = inputColor;
    }

The hardware pipeline expects the first vec4 output from the Fragment
Shader to represent an rbga value.  Each element is a float from 0.0 to
1.0.  Alpha channel effects are determined by the **blend mode**.



