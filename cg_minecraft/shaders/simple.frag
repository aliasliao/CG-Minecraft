#version 400 core

in vec3 TexCoord;

out vec4 FragColor;

uniform sampler2DArray texes;

void main() {
	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}