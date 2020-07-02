#version 440 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 nor;

uniform mat4 view_projection;
uniform mat4 model;

void main()
{
	gl_Position = view_projection * model * vec4(pos, 1.0f);
}