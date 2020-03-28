#pragma once

struct Pos {
	float x;
	float y;
	float z;
	float w;
};

struct Col {
	float r;
	float g;
	float b;
	float a;
};

struct Vertex {
	Pos pos{ 0, 0, 0, 1 };
	Col col{ 1.f, 1, 1, 1 };
};

class Game
{
public:
	Game();
	void tick(double dt);
	void render();

private:
	void CheckShader(unsigned int shader);

	Vertex vertices[3] = { 0 };

	float positions[9] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

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
			//FragColor = vec4(0.3, 0.4, 0.5, 1.0);
		} 
	)";



	unsigned int VBO, VAO;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
};

