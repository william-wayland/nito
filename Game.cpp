#include "Game.h"
#include "GL\glew.h"
#include "SDL.h"

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));

	glBindVertexArray(0);
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
	
	glUseProgram(shaderProgram);

	CheckGLError();

	model = glm::mat4(1.0f);  
}

void Game::tick(double dt) {
	// Calc ratio correction based on the two dimensions
	float correction;
	float deviceRatio = 1;
	float virtualRatio = 1;
	float xCorrection = 4;
	float yCorrection = 4;

	if (virtualRatio < deviceRatio) {
		correction = yCorrection;
	}
	else {
		correction = xCorrection;
	}

	// Now when you calc your ortho projection--in this case centered--
// just divide by the correction
	double left = -256 / 2.0f / correction;
	double right = 256 / 2.0f / correction;
	double bottom = -256 / 2.0f / correction;
	double top = 256 / 2.0f / correction;

	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0, 1, 0));

	/*for (auto it = vertices.begin(); it != vertices.end(); ++it) {
		it->pos = it->pos * mvp;
	}*/
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
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 view = glm::lookAt(
		glm::vec3(2, 0, 0), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	 
	glm::mat4 mvp = projection * view * model;
	auto mvp_location = glGetUniformLocation(shaderProgram, "mvp");
	CheckGLError();
	glUniformMatrix4fv(mvp_location, 1, false, &mvp[0][0]);

	CheckGLError();

	// Draw Scene
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// End Draw Scene
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