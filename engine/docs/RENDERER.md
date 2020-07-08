# Unique UV coordinates per triangle

In order to strip down memory usage, it shouldn't be necessary to store each positional attribute up to six times per adjacent triangle inside the vertex buffer.

To allow for each triangle to be drawn via index buffer, each vertex having only one pair of UV coordinates while drawing, there must be at least six runs, each using a different index buffer, to draw only triangles that do not share any vertex in the same run, and another vertex buffer layout, that uses a different pair of UV coordinates within the vertex buffer.

![Vertex Layouts](./graphics/vertex_layouts.svg)

Only the vertex buffer layout and index buffer need to be swapped between draw calls.