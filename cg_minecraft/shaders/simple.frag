#version 400 core

in vec3 TexCoord;

out vec4 FragColor;

uniform sampler2DArray texes;

void main() {
	FragColor = texture(texes, TexCoord);
}