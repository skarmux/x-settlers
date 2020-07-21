#type vertex
#version 460 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in float a_TexIndex;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
flat out float v_TexIndex;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position, 0.0, 1.0);
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
};

#type fragment
#version 460 core

in vec2 v_TexCoord;
flat in float v_TexIndex;

layout(location = 0) out vec4 color;

uniform sampler2D u_Textures[32];

void main()
{
	color = texture(u_Textures[int(v_TexIndex)], v_TexCoord);
};