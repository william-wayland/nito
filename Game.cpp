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


Game::Game(const size_t screen_width, const size_t screen_height, const std::atomic<bool>* keys, const std::atomic<int>* delta_mouse)
	: m_screen_width(screen_width)
	, m_screen_height(screen_height)
	, m_keys(keys)
	, m_delta_mouse(delta_mouse)
{

	//
	vertices[0].pos = { -1.0, -1.0,  1.0, 1.0 };
	vertices[1].pos = { 1.0, -1.0,  1.0, 1.0 };
	vertices[2].pos = { 1.0,  1.0,  1.0, 1.0 };
	vertices[3].pos = { -1.0,  1.0,  1.0, 1.0 };
	vertices[4].pos = { -1.0, -1.0, -1.0, 1.0 };
	vertices[5].pos = { 1.0, -1.0, -1.0, 1.0 };
	vertices[6].pos = { 1.0,  1.0, -1.0, 1.0 };
	vertices[7].pos = { -1.0,  1.0, -1.0, 1.0 };

	vertices[0].col = { 0.5, 0.5, 0.0, 1.0 };
	vertices[1].col = { 0.0, 0.0, 0.0, 1.0 };
	vertices[2].col = { 0.5, 0.5, 0.5, 1.0 };
	vertices[3].col = { 1.0, 0.5, 0.0, 1.0 };
	vertices[4].col = { 0.0, 0.5, 0.0, 1.0 };
	vertices[5].col = { 0.5, 0.5, 1.0, 1.0 };
	vertices[6].col = { 0.5, 0.5, 0.0, 1.0 };
	vertices[7].col = { 0.0, 0.0, 0.5, 1.0 };

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	GLushort cube_elements[36] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
	model = glm::translate(model, glm::vec3(0, 0, 0));
}

static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
static float cameraSpeed = 0.1f;
static float cameraRotationSpeed = 0.5f;
static float yaw = -90, pitch = 0, roll = 0;

void Game::tick(double dt) {

	if (m_keys[SDLK_a]) cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (m_keys[SDLK_d]) cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (m_keys[SDLK_s]) cameraPos -= cameraSpeed * cameraFront;
	if (m_keys[SDLK_w]) cameraPos += cameraSpeed * cameraFront;

	if (m_keys[SDLK_q]) cameraPos -= cameraSpeed * cameraUp;
	if (m_keys[SDLK_e]) cameraPos += cameraSpeed * cameraUp;

	yaw += m_delta_mouse[0] * cameraRotationSpeed;
	pitch += m_delta_mouse[1] * cameraRotationSpeed;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(-glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0, 1, 0));
	
	auto t = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()) / 1000.0;

	vertices[0].col.r = (std::sin(t + 3.14 / 2.0) + 1.0) / 2.0f;
	vertices[0].col.g = (std::sin(1.1 * t + 3.14 / 4.0) + 1.0) / 2.0f;
	vertices[0].col.b = (std::sin(1.5 * t) + 1.0) / 2.0f;

	vertices[1].col.r = (std::sin(2.2 * t + 3.14 / 2.0) + 1.0) / 2.0f;
	vertices[1].col.g = (std::sin(1.1 * t + 3.14 / 4.0) + 1.0) / 2.0f;
	vertices[1].col.b = (std::sin(t) + 1.0) / 2.0f;

	vertices[2].col.r = (std::sin(t - 3.14 / 2) + 1.0) / 2.0f;
	vertices[2].col.g = (std::sin(1.1 * t - 3.14 / 4) + 1.0) / 2.0f;
	vertices[2].col.b = (std::sin(1.5 * t - 1) + 1.0) / 2.0f;
}

void Game::render() {
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), m_screen_width / (float)m_screen_height, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	 
	glm::mat4 mvp = projection * view * model;
	auto mvp_location = glGetUniformLocation(shaderProgram, "mvp");
	glUniformMatrix4fv(mvp_location, 1, false, &mvp[0][0]);
	CheckGLError();

	// Draw Scene
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	int size; glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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