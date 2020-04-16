#pragma once

#include <GL\glew.h>
#include <array>
#include <vector>
#include <map>
#include <memory>
#include <string>

#include "Definitions.h"

// Static class designed to store various vbos of primitives
class Primitives {
public:
	static std::map<std::string, std::pair<GLuint, size_t>> named;
	static GLuint vao;

	static GLuint GetVBO(const std::string& name) {
		return Primitives::named.at(name).first;
	}

	static GLuint GetVertexCount(const std::string& name) {
		return Primitives::named.at(name).second;
	}

	static void BindVAO() {
		glBindVertexArray(vao);
	}

	static void UnbindVAO() {
		glBindVertexArray(0);
	}

	static void Render(const std::string& name) {
		glBindVertexBuffer(0, Primitives::GetVBO(name), 0, sizeof(NormalVertex));
		glDrawArrays(GL_TRIANGLES, 0, Primitives::GetVertexCount(name));
		glBindVertexBuffer(0, 0, 0, 0);
	}

	static void Init() {
		std::vector<NormalVertex> box_vertices = {
			{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f)},
			{glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f)},
			{glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f)},
			{glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f)},
			{glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f)},
			{glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f)},

			{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f, 1.0f)},
			{glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0f,  0.0f, 1.0f)},
			{glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  0.0f, 1.0f)},
			{glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  0.0f, 1.0f)},
			{glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  0.0f, 1.0f)},
			{glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(0.0f,  0.0f, 1.0f)},

			{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f)},
			{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f)},
			{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f)},
			{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f)},
			{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f)},
			{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f)},

			{glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(1.0f,  0.0f,  0.0f)},
			{glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(1.0f,  0.0f,  0.0f)},
			{glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(1.0f,  0.0f,  0.0f)},
			{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f)},
			{glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(1.0f,  0.0f,  0.0f)},
			{glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(1.0f,  0.0f,  0.0f)},

			{glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0f, -1.0f,  0.0f)},
			{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f)},
			{glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0f, -1.0f,  0.0f)},
			{glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0f, -1.0f,  0.0f)},
			{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f)},
			{glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0f, -1.0f,  0.0f)},

			{glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  1.0f,  0.0f)},
			{glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  1.0f,  0.0f)},
			{glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  1.0f,  0.0f)},
			{glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  1.0f,  0.0f)},
			{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f)},
			{glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  1.0f,  0.0f)}
		};

		std::vector<NormalVertex> triangle_vertices = {
			{glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f,  0.0f, 1.0f)},
			{glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.0f,  0.0f, 1.0f)},
			{glm::vec3(-0.5f, -0.5f, 0.0f),  glm::vec3(0.0f,  0.0f, 1.0f)},
		};

		std::vector<NormalVertex> plane_vertices = {
			{glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f,  0.0f, 1.0f)},
			{glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.0f,  0.0f, 1.0f)},
			{glm::vec3(-0.5f, -0.5f, 0.0f),  glm::vec3(0.0f,  0.0f, 1.0f)},

			{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f,  0.0f, 1.0f)},
			{glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f,  0.0f, 1.0f)},
			{glm::vec3(-0.5f, 0.5f, 0.0f),  glm::vec3(0.0f,  0.0f, 1.0f)},
		};

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(NormalVertex), (void*)offsetof(NormalVertex, pos));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(NormalVertex), (void*)offsetof(NormalVertex, nor));

		glBindVertexArray(0);

		ContructPrimitive("box", box_vertices);
		ContructPrimitive("triangle", triangle_vertices);
		ContructPrimitive("plane", plane_vertices);
	}

private: 
	static void ContructPrimitive(const std::string& name, const std::vector<NormalVertex>& vertices) {
		GLuint vao, vbo;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(NormalVertex), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(NormalVertex), (void*)offsetof(NormalVertex, pos));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(NormalVertex), (void*)offsetof(NormalVertex, nor));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		named.emplace(name, std::make_pair(vbo, vertices.size()));
	}
};

std::map<std::string, std::pair<GLuint, size_t>> Primitives::named = std::map<std::string, std::pair<GLuint, size_t>>();
GLuint Primitives::vao = 0;