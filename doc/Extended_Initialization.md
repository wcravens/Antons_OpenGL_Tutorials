# [Extended Initialization](https://antongerdelan.net/opengl/glcontext2.html)

Starting a log file.

Setting a Minimmum OpenGL Version to Use.

Anti-Aliasing.

Window Resolution, Monitor Selection and Full-Screen Mode.

Window resizeing callback.

Printing Parameters from the GL Context

Monitoring the GL State Machine

Once we set a parameter in the OpengGL State Machine it is then globally
available to all OpenGL operations, eg. Drawing Operations.

In GL domain language, setting a state is referred to as "**Binding**"
for buffers, "**enableing**"" for rendering modes, and "**using**" for
shader programs.

Lot of errors originate from misconfiguration or ignorance of (or
forgetting) the  current machine state. Eg. setting a state by
accident,forgetting an unset state, or mixing up the numbering of
indices.

Many state machine variables can be fetched during runtime, as we do in
the GL Paramereter logging that has been impemented. It's superflous to
routinely log many of these, but it's worth remembering this   when
debugging more awkward situation.

Frame Rate Counter

There are really good notes in the main text about early optimization
and the potential insignificance of the reported frame rate until your
application has noticible efficientcy issues.


