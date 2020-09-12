# Tilemap Shader

Split texture coordinate range into segments. Those square segments can then each be split into an L- and R-triangle. Now we have our triangle grid in which we can assign different textures to each triangle. Textures are defined by a 2D position within a texture atlas. The tilemap is a texture storing the atlas positions of the L- and R-triangle within the RG and BA channels respectively, consuming 32 bit of memory per segment.

## Comparison of memory requirements

```Python
#! bin/env/python3

vertex_count = size * size
triangle_count = (size - 1) * (size - 1) * 2
index_count = triangle_count * 3

vertex_conv_size = 20 # 20 Byte [ x, y, s, u, v ]
vertex_indx_size = 60 # 60 Byte [ x, y, s , 6 * [ u, v ] ]
      index_size =  4 # 4 Byte [ i ]

conventional_size = triangle_count * 3 * vertex_conv_size # vertex buffer

indexed_size = ( vertex_count * vertex_indx_size ) # vertex buffer
indexed_size += ( index_count * index_size ) # index buffer
```

size | conventional (MB) | tilemap shader (MB)
---- | ----------------- | -------------------
448  | 23.97708          | 0.799772
1024 | 125.58348         |
4096 | 2012.283          |
8192 | 8051.09772        | 268.378203

The approach allows us to scale the map size higher without running out of memory on many systems. The only drawback being, that the buffer layout needs to be switched multiple times and there need to be five more draw calls to render the entire maps surface.

Whether or not the additional draw calls are too expensive for operation needs to be tested!