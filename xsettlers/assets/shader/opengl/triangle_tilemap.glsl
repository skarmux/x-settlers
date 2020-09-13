#type vertex
#version 460 core

layout (location = 0) in vec2  a_Position;
layout (location = 1) in vec2  a_TexCoord;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position, 0.0, 1.0);
	v_TexCoord  = a_TexCoord;
}

#type fragment
#version 460 core

in vec2 v_TexCoord;

out vec4 color;

uniform sampler2D  u_Atlas;
uniform usampler2D u_TileMap;
uniform sampler2D u_HeightMap;

void main()
{
	ivec2 atlas_size   = textureSize(u_Atlas, 0);
	float tilemap_size = textureSize(u_TileMap, 0).x;
	float map_size     = tilemap_size + 1.0;
	ivec2 tile_size    = ivec2(16,16);

	float height = texture(u_HeightMap, v_TexCoord).r * 0xFF;
	float displacement = height * (1.0 / (map_size * 16.0));
    vec2 texCoord = v_TexCoord + vec2(displacement / 2.0, displacement);

	vec2 scaled_UV = texCoord * vec2(tilemap_size);
	ivec2 cell = ivec2(floor(scaled_UV));
	vec2 cell_UV = fract(scaled_UV);

	/* check if local UV lies within L or R triangle of current cell
	 *
	 * A = (0,0) ; B = (1,1) ; P = cell_UV ; d = (Px-Ax)*(By-Ay)-(Py-Ay)*(Bx-Ax)
	 * d = (cell_UV.x-0)*(1-0)-(cell_UV.y-0)*(1-0) = cell_UV.x - cell_UV.y
	 *
	 * 0,0 +---+      d > 0 : R triangle
	 *    L|-\+|R     d < 0 : L triangle
	 *     +---+ 1,1  d = 0 : exactly on line
	 */
	float d = clamp(sign(cell_UV.x - cell_UV.y), 0.0, 1.0);

	// transform cell coordinate to tile coordinate (different triangle form)
	// +---+    >>+-+    d = 0    | +---+    +---+  d = 1
	// |L\ | -> |/L\| (1-V) * 0.5 | | \R| -> |\R/| V * 0.5
	// +---+    +---+             | +---+    +-+<<
	if(d == 1.0)
		cell_UV.x -= cell_UV.y * 0.5;
	else
		cell_UV.x += (1.0 - cell_UV.y) * 0.5;

	// texture bleed prevention
	float inwards_factor = 1.0 / (tile_size.x * 1.3);
	float new_range = 1.0 - (2 * inwards_factor);
	cell_UV.x = (cell_UV.x * new_range) + inwards_factor;
	cell_UV.y = (cell_UV.y * new_range) + inwards_factor;

	vec2 atlas_UV = (cell_UV / atlas_size.x) * tile_size;

	// calculate atlas offset for current tile index
	uvec4 tile_indices = texelFetch(u_TileMap, cell, 0);
	ivec2 atlas_tex_pos = ivec2(
		tile_indices[(2 * int(d))] * 8,
		tile_indices[(2 * int(d)) + 1] * 16 );
	vec2 atlas_UV_offset = vec2(atlas_tex_pos) / atlas_size.x;

	float above = texture(u_HeightMap, v_TexCoord - (1.0 / atlas_size.y), 0).r;
	float under = texture(u_HeightMap, v_TexCoord + (1.0 / atlas_size.y), 0).r;
	float shade = 0.875f + (above - under) * 6.0f;

	color = texture(u_Atlas, atlas_UV + atlas_UV_offset) * shade;
}