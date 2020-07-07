#type vertex
#version 460 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position, 0.0, 1.0);
	v_TexCoord = a_TexCoord;
};

#type fragment
#version 460 core

in vec2 v_TexCoord;

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TexCoord);
};