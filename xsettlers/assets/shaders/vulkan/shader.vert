#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
} ubo;

layout(location = 0) in vec2  inPosition;
layout(location = 1) in uint  inType;
layout(location = 2) in float inGradient;
layout(location = 3) in vec2  inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = ubo.proj * ubo.model * vec4(inPosition, 0.0, 1.0); // ubo.proj * ubo.view * ubo.model * 
	fragColor = vec3(inGradient);
	fragTexCoord = inTexCoord;
}