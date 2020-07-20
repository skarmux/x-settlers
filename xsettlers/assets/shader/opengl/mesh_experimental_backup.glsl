#type vertex
#version 460 core

layout (location = 0) in float a_Index;
// layout (location = 1) in float a_Height;
// layout (location = 2) in float a_Color;
layout (location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform sampler2D u_HeightBuffer;
//uniform samplerBuffer u_ShadeBuffer;

out vec2 v_TexCoord;
out float v_Color;

void main()
{
	gl_Position = u_ViewProjection * vec4(
		(mod(a_Index, 448.0) * 16.0) + ((447.0 - (a_Index / 448.0)) * 8.0),
		((447.0 - (a_Index / 448.0)) * 9.0) + (texture(u_HeightBuffer, vec2(mod(a_Index, 448.0) / 447.0, a_Index / 448.0 / 447.0)).r * 24.0),
		0.0,
		1.0 );

	v_TexCoord = a_TexCoord;

	v_Color = 1.0f;
};

#type fragment
#version 460 core

in vec2 v_TexCoord;
in float v_Color;

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;

void main()
{
	color = vec4(1.0, 0.0, 1.0, 1.0); // * texture(u_Texture, v_TexCoord) * v_Color;
};