#pragma once

#include <string>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <vector>

class Shader;

class HeightMap {
public:
	HeightMap(const std::string& path);
	HeightMap(int x, int y, float scale);

	void render();
	float heightAt(float x, float y);

private:
	unsigned int m_vbo;
	std::vector<std::vector<float>> m_height_data;
	size_t m_n_vertices;
};