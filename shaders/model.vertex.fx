#version 440 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 nor;
layout(location = 2) in vec2 uv;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform vec3 _eye;

out vec2 TexCoord;
out vec3 normal;
out vec3 FragPos;
out vec3 eye;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	TexCoord = uv;

	normal = normalize(mat3(transpose(inverse(model))) * nor);
	FragPos = vec3(model * vec4(pos, 1.0f));
	eye = _eye;
}