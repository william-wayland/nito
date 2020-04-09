#pragma once

#include <array>
#include <atomic>
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct Vertex {
	glm::vec4 pos{ 0.0f };
	glm::vec4 col{ 1.0f };
};

class Game
{
public:
	Game(const size_t screen_width, const size_t screen_height, const std::atomic<bool>* keys, const std::atomic<int>* delta_mouse);
	void tick(double dt);
	void render();

private:
	void CheckShader(unsigned int shader);

	glm::mat4 model;
	std::array<Vertex, 8> vertices;

	const std::atomic<bool>* m_keys;
	const std::atomic<int>* m_delta_mouse;
	const size_t m_screen_width;
	const size_t m_screen_height;

	const char* vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec4 pos;
		layout (location = 1) in vec4 col;

        uniform mat4 mvp;

		out vec4 aCol;

		void main()
		{
		   gl_Position = mvp * pos;
		   aCol = col;
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

	unsigned int VBO, VAO, ibo;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
};

