#version 440 core
out vec3 FragColor;

in vec3 aNor;
in vec3 FragPos;

void main()
{
	FragColor = abs(normalize(FragPos));
}