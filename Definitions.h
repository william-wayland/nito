#pragma once

#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct Vertex {
	glm::vec4 pos{ 0.0f };
	glm::vec4 col{ 1.0f };
};