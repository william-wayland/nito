#version 440 core
out vec3 FragColor;
in vec3 aCol;

void main()
{
	FragColor = aCol;
}