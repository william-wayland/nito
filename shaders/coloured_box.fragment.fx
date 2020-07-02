#version 440 core
#define M_PI 3.1415926535897932384626433832795

out vec3 FragColor;

uniform vec3 colour;

void main()
{
	FragColor = colour;
}