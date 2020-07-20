#type vertex
#version 460 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in float a_Shade;
layout (location = 3) in float a_TexIndex;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
flat out float v_TexIndex;
out float v_Shade;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position, 0.0, 1.0);
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_Shade = a_Shade;
};

#type fragment
#version 460 core

in vec2 v_TexCoord;
flat in float v_TexIndex;
in float v_Shade;

uniform sampler2D u_Textures[16];

layout(location = 0) out vec4 color;

void main()
{
	color = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * vec4(vec3(v_Shade), 1.0);
};