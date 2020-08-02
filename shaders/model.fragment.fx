#version 440 core

#define M_PI 3.1415926535897932384626433832795

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform sampler2D texture_1;
uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_pos;

uniform Material material;

uniform float gamma = 2.2;

out vec3 FragColor;

in vec2 TexCoord;
in vec3 normal;
in vec3 FragPos;
in vec3 eye;

float near = 0.1;
float far = 100.0;

struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirectionalLight dir_light;

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define N_POINT_LIGHT 8
uniform int n_point_light = N_POINT_LIGHT;
uniform PointLight point_lights[N_POINT_LIGHT];

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 view_dir, vec3 frag_pos)
{
	vec3 light_dir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, light_dir), 0.0);
	// specular shading
	vec3 halfway_dir = normalize(light_dir + view_dir);
	float spec = pow(max(dot(normal, halfway_dir), 0.0), material.shininess);
	// combine results
	vec3 ambient = light.ambient * material.diffuse;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 view_dir, vec3 frag_pos)
{
	vec3 light_dir = normalize(light.position - frag_pos);
	// diffuse shading
	float diff = max(dot(normal, light_dir), 0.0);
	// specular shading
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position - frag_pos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));
	// combine results
	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

void main()
{
	vec3 view_dir = normalize(eye - FragPos);
	vec3 light = CalcDirLight(dir_light, normal, view_dir, FragPos);

	for (int i = 0; i < n_point_light; i++) {
		light += CalcPointLight(point_lights[i], normal, view_dir, FragPos);
	}

	FragColor = pow(light, vec3(1.0/gamma));
}