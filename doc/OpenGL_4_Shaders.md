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

## Minimal Shader Code

Recall that each individual shader is a small c-like programm that.  The
shaders then need to be linked together to create a full GPU hardware
piepeline program.

Here are the basic steps:

1.  load a *vertex shader file* and *fragment shader file* from disk and store each in a separate C string
2.  call `glCreateShader` twice; for 1 vertex and 1 fragment shader index
3.  call `glShaderSource` to copy code from a string for each of the above
4.  call `glCompileShader` for both shader indices
5.  call `glCreateProgram` to create an index to a new program
6.  call `glAttachShader twice,` to attach both shader indices to the program
7.  call `glLinkProgram`
8.  call `glGetUniformLocation` to get the unique location (memory address) of the `uniform` variable called `inputColour`
9.  call `glUseProgram` to switch to your shader before calling
10. call `glUniform4f(location, r,g,b,a)` to assign an initial colour to your fragment shader (e.g. `glUniform4f(colour_loc, 1.0f, 0.0f, 0.0f, 1.0f)` for red)

You will need to keep track of the ID's (indexes) that are created by
`glCreateProgram` and any `uniform` locations you need.  They should
both be `GLuint`s.

Be aware that the `glUniform` function operates on the last program that
was set active by the `glUseProgram( id )` function.

## OpenGL Shader Functions

[Shader Functions - Official GLSL Reference card ](https://www.khronos.org/files/opengl42-quick-reference-card.pdf)

### Most regularly used **Shader functions**:

`glCreateShader()`: create an OpenGL shader variable and return a reference index (`GLuint`)
`glShaderSource()`: copy shader code from C string into an OpenGL shader variable.
`glCompileShader()`: compile an OpenGL shader variable
`glGetShaderiv()`: return an `int` value for a shader parameter[1]

(1) Valid shader parameters are `GL_SHADER_TYPE`, `GL_DELETE_STATUS`,
`GL_COMPILE_STATUS`, `GL_INFO_LOG_LENGTH`, and
`GL_SHADER_SOURCE_LENGTH`.

### Most regularly used **Program functions**:

`glCreateProgram()`: create an OpenGL program variable and return a reference (index) as `GLuint`.
`glAttacheShader()`: attach a compiled OpenGL shader variable to a shader program variable.
`glLinkProgram()`: link the shaderes into a complete program once all indivividual shaders have been attached.
`glValidateProgram()`: check if a program is ready.  information is stored in a log.
`glGetProgramiv()`: return an `int` value for a program parameter[2]
`glGetProgramInfoLog()`: information from `glLinkProgram` and `glValidateProgram` returned as a C-string.
`glUseProgram()`: switch hardware pipeline to a specified shader program.
`glGetActiveAttrib()`: get details of an active (linked) attribute in the program. [3]
`glGetAttribLocation()`: get the unique index identifier of a named attribute.
`glGetuniformLocation()`: get the unique index identifier of a named `uniform` variable.
`glGetActiveUniform()`: get details of an active (linked) `uniform` varible in the program. [3]
`glUniform{1,2,3,4}{i,f,d}()`: Set the value of a `uniform` variable.  *function name varies by dimensionality and type*.
`glUniform{1,2,3,4}{i,f,d}v()`: Same as above but with a whole vector (`array`) of values at a time.
`glUniformMatrix{2,3,4}{f,d}v()`: same as above but for n `x` n matricies of 2,3, or 4 elements.




(2) Valid program parameters are: `GL_DELETE_STATUS`, `GL_LINK_STATUS`,
`GL_VALIDATE_STATUS`, `GL_INFO_LOG_LENGTH`, `GL_ATTACHED_SHADERS`,
`GL_ACTIVE_ATTRIBUTES`, `GL_ACTIVE_ATTRIBUTE_MAX_LENGTH`,
`GL_ACTIVE_UNIFORMS`, and `GL_ACTIVE_UNIFORM_MAX_LENGTH`.

(3)`glGetProgramiv` with `GL_ACTIVE_ATTRIBUTES` will give you a list of active attributes.
