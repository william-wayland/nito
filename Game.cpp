#include "Game.h"
#include "GL\glew.h"

#include <iostream>
#include <chrono>

const char* GetGLErrorStr(GLenum err)
{
	switch (err)
	{
	case GL_NO_ERROR:          return "No error";
	case GL_INVALID_ENUM:      return "Invalid enum";
	case GL_INVALID_VALUE:     return "Invalid value";
	case GL_INVALID_OPERATION: return "Invalid operation";
	case GL_STACK_OVERFLOW:    return "Stack overflow";
	case GL_STACK_UNDERFLOW:   return "Stack underflow";
	case GL_OUT_OF_MEMORY:     return "Out of memory";
	default:                   return "Unknown error";
	}
}

void CheckGLError()
{
	while (true)
	{
		const GLenum err = glGetError();
		if (GL_NO_ERROR == err)
			break;

		std::cout << "GL Error: " << GetGLErrorStr(err) << std::endl;

	}
}


Game::Game() {

	//
	vertices[0].pos = { -0.5, -0.5, 0.0, 1.0 };
	vertices[1].pos = { 0.5, -0.5, 0.0, 1.0 };
	vertices[2].pos = { 0.0, 0.5, 0.0, 1.0 };

	vertices[0].col = { 0.5, 0.5, 0.0, 1.0 };
	vertices[1].col = { 0.0, 0.5, 0.0, 1.0 };
	vertices[2].col = { 0.5, 0.5, 0.5, 1.0 };

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));

	

	// shaders
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
	glCompileShader(vertexShader);
	CheckShader(vertexShader);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
	glCompileShader(fragmentShader);
	CheckShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	glUseProgram(shaderProgram);

	CheckGLError();

	/*glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));
	glEnableVertexAttribArray(1);*/
}

void Game::tick(double dt) {
	glm::dmat4 mod = glm::rotate(glm::dmat4(1.0), 0.01, glm::dvec3(0, 1, 0));
	/*mod = glm::rotate(mod, 0.01f, glm::vec3(1, 0, 0));
	mod = glm::rotate(mod, 0.01f, glm::vec3(0, 0, 1));*/

	for (int i = 0; i < 3; i++) {
		vertices[i].pos = vertices[i].pos * mod;
	}
	auto t = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()) / 1000.0;

	vertices[0].col.r = (std::sin(t + 3.14 / 2.0) + 1.0) / 2.0;
	vertices[0].col.g = (std::sin(1.1 * t + 3.14 / 4.0) + 1.0) / 2.0;
	vertices[0].col.b = (std::sin(1.5 * t) + 1.0) / 2.0;

	vertices[1].col.r = (std::sin(2.2 * t + 3.14 / 2.0) + 1.0) / 2.0;
	vertices[1].col.g = (std::sin(1.1 * t + 3.14 / 4.0) + 1.0) / 2.0;
	vertices[1].col.b = (std::sin(t) + 1.0) / 2.0;

	std::cout << t << std::endl;
	vertices[2].col.r = (std::sin(t - 3.14 / 2) + 1.0) / 2.0;
	vertices[2].col.g = (std::sin(1.1 * t - 3.14 / 4) + 1.0) / 2.0;
	vertices[2].col.b = (std::sin(1.5 * t - 1) + 1.0) / 2.0;
}

void Game::render() {
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Game::CheckShader(unsigned int shader) {

	int success = 0;
	char infoLog[512] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, 0, infoLog);
		std::cout << "ERROR: " << infoLog << std::endl;
	}
}