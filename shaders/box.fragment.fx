#version 440 core
#define M_PI 3.1415926535897932384626433832795


out vec3 FragColor;

in vec3 aNor;
in vec3 FragPos;
in vec3 eye;

void main()
{
	vec3 dir = normalize(FragPos - eye);
	float angle_between_eye_and_norm = acos(dot(aNor, dir) / ((length(aNor) * length(dir))));

	if (dot(aNor, dir) > 0)
		discard;
	
	FragColor = abs(normalize(FragPos));
}