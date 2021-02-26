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

