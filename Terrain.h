#pragma once
#include "Definitions.h"

#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <array>

class Shader;

class Terrain
{
public:
	Terrain();
	void tick();
	void render(Shader& shaderProgram, const glm::mat4& pv);
private:
	glm::mat4 model;
	std::array<ColouredVertex, 4> vertices;

	unsigned int vbo, vao, ibo;
};

