#pragma once
#include "Definitions.h"

#include <array>

class Terrain
{
public:
	Terrain();
	void tick();
	void render(int shaderProgram, const glm::mat4& pv);
private:
	glm::mat4 model;
	std::array<Vertex, 4> vertices;

	unsigned int vbo, vao, ibo;
};

