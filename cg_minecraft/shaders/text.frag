#version 400 core

in vec2 texpos;

out vec4 fragColor;

uniform sampler2D tex;
uniform vec4 color;

void main() {
	fragColor = vec4(1, 1, 1, texture2D(tex, texpos).a) * color;
}