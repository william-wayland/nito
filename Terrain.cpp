#include "Terrain.h"
#include "GL\glew.h"
#include "SDL.h"

#include "Shader.h"

#include <array>

Terrain::Terrain() {
	vertices[0].pos = { -1.0f, 0.0f, -1.0f};
	vertices[1].pos = { -1.0f, 0.0f, 1.0f};
	vertices[2].pos = { 1.0f, 0.0f, 1.0f};
	vertices[3].pos = { 1.0f, 0.0f, -1.0f};

	vertices[0].col = { 0.0f, 0.5f, 0.1f};
	vertices[1].col = { 0.0f, 0.9f, 0.1f};
	vertices[2].col = { 0.1f, 0.3f, 0.1f};
	vertices[3].col = { 0.1f, 0.4f, 0.1f};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	GLushort elements[6] = {
		0, 1, 2,
		2, 3, 0,
	};

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColouredVertex), (void*)offsetof(ColouredVertex, pos));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ColouredVertex), (void*)offsetof(ColouredVertex, col));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	model = glm::mat4(1.0f);
	model = glm::scale(model, 5.0f * glm::vec3(1, 0, 1));
}

void Terrain::tick() {

}

void Terrain::render(Shader& shader, const glm::mat4& pv) {
	shader.use();
	glm::mat4 mvp = pv * model;
	shader.setMat4("mvp", mvp);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	int size; glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}