#pragma once
#include "Definitions.h"

#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <array>
#include "HeightMap.h"

class Shader;

class Terrain
{
public:
	Terrain(const std::string& path, const glm::vec3& translate);
	Terrain(int width, int height, float scale, const glm::vec3& translate);
	void tick();
	void render(Shader& shaderProgram);
private:
	glm::mat4 m_model;
	HeightMap m_hm;
};

