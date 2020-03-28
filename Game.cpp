#include "Game.h"
#include "GL\glew.h"
#include <iostream>

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
	vertices[0].pos = { -0.5f, -0.5f, 0.0f, 1.0f };
	vertices[1].pos = { 0.5f, -0.5f, 0.0f, 1.0f };
	vertices[2].pos = { 0.0f, 0.5f, 0.0f, 1.0f };

	vertices[0].col = { 0.5f, 0.5f, 0.0f, 1.0f };
	vertices[1].col = { 0.0f, 0.5f, 0.0f, 1.0f };
	vertices[2].col = { 0.5f, 0.5f, 0.5f, 1.0f };

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

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
	

	CheckGLError();

	/*glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));
	glEnableVertexAttribArray(1);*/
}

void Game::tick(double dt) {
	
}

void Game::render() {
	CheckGLError();
	glUseProgram(shaderProgram);
	CheckGLError();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	CheckGLError();
	glDrawArrays(GL_TRIANGLES, 0, 3);
	CheckGLError();
	
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