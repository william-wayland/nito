#pragma once

#include <string>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Shader;

class HeightMap {
public:
	HeightMap(const std::string& path);
	HeightMap(int x, int y, float scale);

	void render();

private:
	unsigned int m_vbo;
	size_t m_n_vertices;
};