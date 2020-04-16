#pragma once

#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

struct Vertex {
	glm::vec3 pos{ 0.0f };
};

struct ColouredVertex {
	glm::vec3 pos{ 0.0f };
	glm::vec3 col{ 1.0f };
};

struct NormalVertex {
	glm::vec3 pos{ 0.0f };
	glm::vec3 nor{ 0.0f };
};