# version 400 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in ivec3 textureVec;

out vec3 FragPos;
out ivec3 TextureVec;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main ()
{
	FragPos	= vec3(model * vec4(vertPos, 1.0));
	TextureVec = textureVec;
	Normal = mat3(transpose(inverse(model))) * normal;

	gl_Position = projection * view * model * vec4(FragPos, 1.0);
}