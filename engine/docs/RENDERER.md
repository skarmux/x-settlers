# Unique UV coordinates per triangle (Indexed Rendering)

In order to strip down memory usage, it shouldn't be necessary to store each positional attribute up to six times per adjacent triangle inside the vertex buffer.

To allow for each triangle to be drawn via index buffer, each vertex having only one pair of UV coordinates while drawing, there must be at least six runs, each using a different index buffer, to draw only triangles that do not share any vertex in the same run, and another vertex buffer layout, that uses a different pair of UV coordinates within the vertex buffer.

![Vertex Layouts](./graphics/vertex_layouts.svg)

Only the vertex buffer layout and index buffer need to be swapped between draw calls.

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

Map Size | Conventional (MB) | Indexed (MB) | Indices Only (MB)
-------- | ----------------- | ------------ | -----------------
448      | 23.97708          | 16.837656    | 14.586952
1024     | 125.58348         | 88.031256    | 76.398664
4096     | 2012.283          | 1409.08956   | 1224.147016
8192     | 8051.09772        | 5636.751384  | 4897.767496

The approach allows us to scale the map size higher without running out of memory on many systems. The only drawback being, that the buffer layout needs to be switched multiple times and there need to be five more draw calls to render the entire maps surface.

Whether or not the additional draw calls are too expensive for operation needs to be tested!

## Filling six Index Buffers based on Map Size

Let's say we want to fill index buffer [ 0 ]. For that, we only want to store indices for the vertices that form the green triangles up there in the overview graphic. The index buffers can only be filled correctly with the size of the map known beforehand. Other solutions would result in huge empty spaces in memory, which could be fine if we want to reserve memory for the game anyway, but for now, we are targeting a small memory footprint.

First we're looping through each pair of two triangles (segments) of the map, with the map size as outer limits.

![Index buffer Sorting](./graphics/index_buffer_sorting.svg)

Numbering the triangles reveals a pattern to follow. Looking at the upwards pointing triangles: Each column increases the number by 2, each row increases it by 2 as well. Therefore we can implement an index_buffer_id value to be updated/reset after each for-loop cycle.

```C++
uint32_t bi = 0; // [0-TRIS_PER_VERTEX]

for (uint32_t row = 0; row < size - 1; row++)
{
	for (uint32_t col = 0; col < size - 1; col++)
	{
		//   (0)--- 1
		//   / \ B /
		//  / A \ /
		// 2 --- 3

		indices[bi].insert(indices[bi].end(), {
			(size * row) + col,				// index 0
			(size * row) + col + size + 1,  // index 3
			(size * row) + col + size		// index 2
			}); // A: 0 > 3 > 2 (clockwise)

		indices[bi + 1].insert(indices[bi + 1].end(), {
			(size * row) + col,				// index 0
			(size * row) + col + 1,			// index 1
			(size * row) + col + size + 1	// index 3
			}); // B: 0 > 1 > 3 (clockwise)

		bi = (bi + 2) % TRIS_PER_VERTEX;
	}

	bi = (row * 2) % TRIS_PER_VERTEX;
}
```