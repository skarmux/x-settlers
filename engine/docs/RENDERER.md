# Unique UV coordinates per triangle

In order to strip down memory usage, it shouldn't be necessary to store each positional attribute up to six times per adjacent triangle inside the vertex buffer.

To allow for each triangle to be drawn via index buffer, each vertex having only one pair of UV coordinates while drawing, there must be at least six runs, each using a different index buffer, to draw only triangles that do not share any vertex in the same run, and another vertex buffer layout, that uses a different pair of UV coordinates within the vertex buffer.

![Vertex Layouts](./graphics/vertex_layouts.svg)

Only the vertex buffer layout and index buffer need to be swapped between draw calls.

## Comparison of memory requirements

```
triangle_count = (size - 1) * (size - 1) * 2
vertex_count = size * size
vertex_size = 20 Byte [ x, y, s, u, v ]
index_count = triangle_count * 3

conv_size = triangle_count * 3 * vertex_size

indx_size = ( vertex_count * 12 Byte [ x, y, s ] )
+ ( vertex_count * 6 * 8 Byte [ u, v ] )
+ ( index_count * 4 Byte [ index ] )
```

Map Size | Conventional (MB) | Indexed (MB)
-------- | ------------ | -------
448 | ~24 | ~17
1024 | ~126 | ~88
4096 | ~2012 | ~1409
8192 | ~8051 | ~5637

The approach allows us to scale the map size higher without running out of memory on many systems. The only drawback being, that the buffer layout needs to be switched multiple times and there need to be five more draw calls to render the entire maps surface.

Whether or not the additional draw calls are too expensive for operation needs to be tested!