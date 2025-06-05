#include "HeightMap.h"

#include <iostream>
#include <algorithm>
#include <GL\glew.h>

#include <vector>
#include <numeric>
#include "Definitions.h"
#include "Shader.h"

#include "Logger.h"

#include "stb_image.h"

#include "imgui/imgui.h"

HeightMap::HeightMap(const std::string& path) {
	int width, height, channels;
	unsigned char* image = stbi_load(path.c_str(), &width, &height, &channels, 1);

	LOG(DEBUG) << "Loading Height Map (w: " << width << ", h:" << height << ", c:" << channels << ")";

	std::vector<NormalVertex> vertices;

	auto n = glm::vec3(0, 1, 0);
	auto wscale = 1.0f;
	auto vscale = 1.0f / 16.0f;

	for (int y = 0; y < height - 1; y++) {
		for (int x = 0; x < width - 1; x++) {
			float h1 = image[x + y * width];
			float h2 = image[(x + 1) + y * width];
			float h3 = image[x + (y + 1) * width];
			float h4 = image[(x + 1) + (y + 1) * width];
			float h5 = (h1 + h2 + h3 + h4) / 4.0f;

			auto p1 = glm::vec3(wscale * x, vscale * h1, wscale * y);
			auto p2 = glm::vec3(wscale * (x + 1), vscale * h2, wscale * y);
			auto p3 = glm::vec3(wscale * x, vscale * h3, wscale * (y + 1));
			auto p4 = glm::vec3(wscale * (x + 1), vscale * h4, wscale * (y + 1));
			auto p5 = glm::vec3(wscale * (x + 0.5f), vscale * h5, wscale * (y + 0.5f));

			auto fn1 = glm::normalize(glm::cross(p5 - p2, p5 - p1));
			auto fn2 = glm::normalize(glm::cross(p5 - p4, p5 - p2));
			auto fn3 = glm::normalize(glm::cross(p5 - p3, p5 - p4));
			auto fn4 = glm::normalize(glm::cross(p5 - p1, p5 - p3));

			auto n1 = 0.5f * (fn1 + fn4);
			auto n2 = 0.5f * (fn1 + fn2);
			auto n3 = 0.5f * (fn3 + fn4);
			auto n4 = 0.5f * (fn2 + fn3);
			auto n5 = 0.25f * (fn1 + fn2 + fn3 + fn4);

			vertices.push_back({ p1, n1 }); vertices.push_back({ p2, n2 }); vertices.push_back({ p5, n5 });
			vertices.push_back({ p2, n2 }); vertices.push_back({ p4, n4 }); vertices.push_back({ p5, n5 });
			vertices.push_back({ p3, n3 }); vertices.push_back({ p4, n4 }); vertices.push_back({ p5, n5 });
			vertices.push_back({ p3, n3 }); vertices.push_back({ p1, n1 }); vertices.push_back({ p5, n5 });
		}
	}

	std::cout << "size: " << vertices.size() << std::endl;

	m_height_data.reserve(height);

	for (int y = 0; y < height; y++) {
		m_height_data.push_back(std::move(std::vector<float>()));
		m_height_data[y].reserve(width);
		for (int x = 0; x < width; x++) {
			m_height_data[y].push_back(vscale * image[x + y * width]);
		}
	}

	m_n_vertices = vertices.size();
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(NormalVertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_vbo = vbo;
	stbi_image_free(image);
}

HeightMap::HeightMap(int width, int height, float scale)
{
	std::vector<NormalVertex> vertices;

	auto n = glm::vec3(0, 1, 0);

	for (int y = 0; y < height - 1; y++) {
		for (int x = 0; x < width - 1; x++) {

			auto p1 = glm::vec3(scale * x, 0, scale * y);
			auto p2 = glm::vec3(scale * (x + 1), 0, scale * y);
			auto p3 = glm::vec3(scale * x, 0, scale * (y + 1));
			auto p4 = glm::vec3(scale * (x + 1), 0, scale * (y + 1));

			auto n = glm::vec3(0, 1, 0);

			vertices.push_back({ p1, n }); vertices.push_back({ p3, n }); vertices.push_back({ p2, n });
			vertices.push_back({ p3, n }); vertices.push_back({ p2, n }); vertices.push_back({ p4, n });
		}
	}

	m_n_vertices = vertices.size();
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(NormalVertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_vbo = vbo;
}

void HeightMap::render() {
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexBuffer(0, m_vbo, offsetof(NormalVertex, pos), sizeof(NormalVertex));
	glBindVertexBuffer(1, m_vbo, offsetof(NormalVertex, nor), sizeof(NormalVertex));
	glDrawArrays(GL_TRIANGLES, 0, m_n_vertices);
	glBindVertexBuffer(0, 0, 0, 0);
}

float HeightMap::heightAt(float x, float z) {
	// Bi lerp
	if (z > m_height_data.size() || z < 0 || x > m_height_data[0].size() || x < 0) return 0;

	const float fx = std::floor(x);
	const float cx = std::ceil(x);
	const float fz = std::floor(z);
	const float cz = std::ceil(z);

	if (cz > m_height_data.size() - 1 || cz < 0 || cx > m_height_data[0].size() - 1 || cx < 0) return 0;

	if (fx == cx && fz == cz) {
		return m_height_data[fz][fx];
	}
	if (fx == cx) {
		return m_height_data[fz][fx] * (cz - z) + m_height_data[cz][fx] * (z - fz);
	}

	if (fz == cz) {
		return m_height_data[fz][fx] * (cx - x) + m_height_data[fz][cx] * (x - fx);
	}

	// Lerp x0z0 -> x1z0 - Height along z lower edge
	const float hz0 = lerp(m_height_data[fz][fx], m_height_data[fz][cx], cx - x);

	// Lerp x0z1 -> x1z1 - Height along z upper edge
	const float hz1 = lerp(m_height_data[cz][fx], m_height_data[cz][cx], cx - x);

	// Lerp along z axis using derived heights from x axis
	return lerp(hz0, hz1, cz - z);
}
