# Unique UV coordinates per triangle

In order to strip down memory usage, it shouldn't be necessary to store each positional attribute up to six times per adjacent triangle inside the vertex buffer.

To allow for each triangle to be drawn via index buffer, each vertex having only one pair of UV coordinates while drawing, there must be at least six runs, each using a different index buffer, to draw only triangles that do not share any vertex in the same run, and another vertex buffer layout, that uses a different pair of UV coordinates within the vertex buffer.

![Vertex Layouts](./graphics/vertex_layouts.svg)

Only the vertex buffer layout and index buffer need to be swapped between draw calls.

## Comparison of memory requirements

```Python
triangle_count = (size - 1) * (size - 1) * 2
vertex_count = size * size
index_count = triangle_count * 3

vertex_conv_size = 20 # 20 Byte [ x, y, s, u, v ]
vertex_indx_size = 60 # 60 Byte [ x, y, s , 6*[ u, v ] ]
index_size = 4 # 4 Byte [ i ]

conventional_size = triangle_count * 3 * vertex_conv_size # vertex buffer

indexed_size = ( vertex_count * vertex_indx_size ) # vertex buffer
indexed_size += ( index_count * index_size ) # index buffer
```

Map Size | Conventional (MB) | Indexed (MB)
-------- | ------------ | -------
448 | 23.97708 | 16.837656
1024 | 125.58348 | 88.031256
4096 | 2012.283 | 1409.08956
8192 | 8051.09772 | 5636.751384

The approach allows us to scale the map size higher without running out of memory on many systems. The only drawback being, that the buffer layout needs to be switched multiple times and there need to be five more draw calls to render the entire maps surface.

Whether or not the additional draw calls are too expensive for operation needs to be tested!