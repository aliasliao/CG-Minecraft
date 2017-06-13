#version 400 core

layout (location = 0) in vec3 vertCoord;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 texCoord;

out vec3 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * model * view * vec4(vertCoord, 1.0);
	TexCoord = texCoord;
}