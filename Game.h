#pragma once

#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct Vertex {
	glm::dvec4 pos{ 0.0f };
	glm::dvec4 col{ 1.0f };
};

class Game
{
public:
	Game();
	void tick(double dt);
	void render();

private:
	void CheckShader(unsigned int shader);

	Vertex vertices[3];

	const char* vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec4 aPos;
		layout (location = 1) in vec4 Col;

		out vec4 aCol;

		void main()
		{
		   gl_Position = vec4(aPos.x, aPos.y, aPos.z, aPos.w);
		   aCol = Col;
		}
	)";

	const char* fragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;
		in vec4 aCol;

		void main()
		{
			FragColor = aCol;
		} 
	)";

	unsigned int VBO, VAO;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
};

