#include "Terrain.h"
#include "GL\glew.h"
#include "SDL.h"

#include "Shader.h"

#include <array>

Terrain::Terrain(const std::string& path, const glm::vec3& translate)
	: m_hm(path)
{
	m_model = glm::mat4(1.0f);
	m_model = glm::translate(m_model, translate);
}

Terrain::Terrain(int width, int height, float scale, const glm::vec3& translate)
	: m_hm(width, height, scale)
{
	m_model = glm::mat4(1.0f);
	m_model = glm::translate(m_model, translate);
}

void Terrain::tick() {

}

void Terrain::render(Shader& shader) {
	static auto object_colour = glm::vec3(1.0f, 1.0f, 1.0f);

	shader.setMat4("model", m_model);

	shader.setVec3("object_color", object_colour);
	
	
	m_hm.render();
}