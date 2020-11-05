#include "Game.h"
#include "GL\glew.h"
#include "SDL.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include "Misc.h"
#include "Primitives.h"
#include "imgui/imgui.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include "tiny_gltf.h"


static auto lightTransform = glm::mat4(1);

glm::mat4 x_trans = glm::mat4(1);
glm::mat4 y_trans = glm::mat4(1);
glm::mat4 z_trans = glm::mat4(1);

auto then = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

std::string print(glm::vec3 v) {
	return "(" + std::to_string(v.x) + ", " + std::to_string(v.z) + ", " + std::to_string(v.z) + ")";
}

Game::Game(const size_t screen_width, const size_t screen_height, const std::atomic<bool>* keys, const std::atomic<int>* mouse_delta)
	: m_screen_width(screen_width)
	, m_screen_height(screen_height)
	, m_keys(keys)
	, m_mouse_delta(mouse_delta)
	, m_terrain("data/height_map_test.png", glm::vec3(0, 0, 0))
	, m_flat_land(100, 100, 1, glm::vec3(-50, 0, -50))
	, m_camera(glm::vec3(0.001f, 2.0f, 0.001f), keys, mouse_delta)
	, m_camera_speed(30.0f)
	, m_default_shader("shaders/default.vertex.fx", "shaders/default.fragment.fx")
	, m_box_shader("shaders/box.vertex.fx", "shaders/box.fragment.fx")
	, m_coloured_box_shader("shaders/coloured_box.vertex.fx", "shaders/coloured_box.fragment.fx")
    , m_model_shader("shaders/model.vertex.fx", "shaders/model.fragment.fx")
{
	Primitives::Init();

	x_trans = glm::translate(x_trans, glm::vec3(5, 0, 0));
	y_trans = glm::translate(y_trans, glm::vec3(0, 5, 0));
	z_trans = glm::translate(z_trans, glm::vec3(0, 0, 5));

	x_trans = glm::scale(x_trans, glm::vec3(10, 0.1, 0.1));
	y_trans = glm::scale(y_trans, glm::vec3(0.1, 10, 0.1));
	z_trans = glm::scale(z_trans, glm::vec3(0.1, 0.1, 10));

	lightTransform = glm::rotate(lightTransform, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	lightTransform = glm::scale(lightTransform, 8.0f * glm::vec3(1, 1, 1));

	m_model_shader.use();
	m_point_lights = m_model_shader.getInt("n_point_light");

}

void Game::tick(float dt) {
	m_camera.setCameraSensitivity(m_camera_speed);
	m_camera.tick(dt);
	m_terrain.tick();

	std::cout << "(" << m_camera.pos().x << ", " << m_camera.pos().z << ") - " << m_terrain.heightAt(m_camera.pos().x, m_camera.pos().z) << std::endl;
}

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  6.2f,  2.0f),
	glm::vec3(2.3f, 7.3f, -4.0f),
	glm::vec3(-4.0f,  5.0f, -12.0f),
	glm::vec3(0.0f,  4.0f, -3.0f),
	glm::vec3(1.0f,  4.0f, -3.0f),
	glm::vec3(10.0f,  4.0f, -3.0f),
	glm::vec3(5.0f,  4.0f, 3.0f),
	glm::vec3(4.0f,  3.0f, -1.0f),
	glm::vec3(5.0f,  4.0f, -2.0f)
};

void Game::render(bool menu) {
	static float fov = 60.0;
	static float scale = 1.0f;
	static auto light_colour = glm::vec3(1.f, 1.0f, 1.0f);
	static auto light_dir = glm::vec3(-0.2f, -1.0f, -0.3f);
	static auto gamma = 1.2f;

	if (menu) {
		ImGui::Begin("Nito");  
		ImGui::SliderFloat("fov", &fov, 45.0f, 90.0f);
		ImGui::SliderFloat("scale", &scale, 0.01f, 10.0f);
		ImGui::SliderFloat("gamma", &gamma, 0.0f, 4.0f);
		ImGui::SliderFloat3("light colour", glm::value_ptr(light_colour), 0, 1);
		ImGui::SliderFloat3("light dir", glm::value_ptr(light_dir), -1, 1);

		ImGui::Spacing();

		ImGui::SliderFloat("Mouse Sensitivity", &m_camera_speed, 0.01f, 100.0f);

		ImGui::Spacing();

		ImGui::SliderFloat3("Eye Pos", glm::value_ptr(m_camera.pos()), 0, 2);

		ImGui::End();  
	}
	
	glm::mat4 projection = glm::perspective(glm::radians(fov), m_screen_width / (float)m_screen_height, 1.0f, 100.0f);
	for (int i = 0; i < 1; i++) {
		glm::mat4 view = m_camera.lookAt();
		glm::mat4 pv = projection * view;

		Primitives::BindVAO();

		// Light
		m_coloured_box_shader.use();
		m_coloured_box_shader.setMat4("view_projection", pv);
		m_coloured_box_shader.setVec3("colour", light_colour);

		for (int i = 0; i < m_point_lights; i++) {
			auto model = glm::translate(glm::mat4(1.0f), pointLightPositions[i]);
			m_coloured_box_shader.setMat4("model", model);
			Primitives::Render("box");
		}

		m_model_shader.use();
		m_model_shader.setMat4("view", view);
		m_model_shader.setMat4("projection", projection);

		m_model_shader.setVec3("light.ambient", 0.2f * light_colour);
		m_model_shader.setVec3("light.diffuse", 0.5f * light_colour); // darken diffuse light a bit
		m_model_shader.setVec3("light.specular", 0.5f * light_colour);
		m_model_shader.setVec3("_eye", m_camera.pos());
		m_model_shader.setVec3("light_color", light_colour);

		m_model_shader.setVec3("material.ambient", glm::vec3(0.05375, 0.05, 0.06625));
		m_model_shader.setVec3("material.diffuse", glm::vec3(0.58275, 0.17, 0.22525));
		m_model_shader.setVec3("material.specular", glm::vec3(0.832741, 0.528634, 0.546435));
		m_model_shader.setFloat("material.shininess", 38.4f);

		m_terrain.render(m_model_shader);

		m_model_shader.setVec3("dir_light.direction", light_dir);
		m_model_shader.setVec3("dir_light.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		m_model_shader.setVec3("dir_light.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		m_model_shader.setVec3("dir_light.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		m_model_shader.setVec3("material.ambient", glm::vec3(0.05375, 0.05, 0.16625));
		m_model_shader.setVec3("material.diffuse", glm::vec3(0.28275, 0.27, 0.82525));
		m_model_shader.setVec3("material.specular", glm::vec3(0.532741, 0.528634, 0.946435));
		m_model_shader.setFloat("material.shininess", 128.4f);

		m_model_shader.setMat4("model", glm::scale(glm::translate(glm::mat4(1), glm::vec3(-3, 3, -3)), glm::vec3(100, 0.1, 100)));
		m_model_shader.setFloat("gamma", gamma);

		for (int i = 0; i < m_point_lights; i++) {
			m_model_shader.setVec3("point_lights[" + std::to_string(i) + "].position", pointLightPositions[i]);
			m_model_shader.setVec3("point_lights[" + std::to_string(i) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f) * light_colour);
			m_model_shader.setVec3("point_lights[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f, 0.8f, 0.8f) * light_colour);
			m_model_shader.setVec3("point_lights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f) * light_colour);
			m_model_shader.setFloat("point_lights[" + std::to_string(i) + "].constant", 0.5f);
			m_model_shader.setFloat("point_lights[" + std::to_string(i) + "].linear", 0.09f);
			m_model_shader.setFloat("point_lights[" + std::to_string(i) + "].quadratic", 0.032f);
		}

		m_flat_land.render(m_model_shader);

		m_box_shader.use();
		m_box_shader.setMat4("view_projection", pv);
		m_box_shader.setVec3("_eye", m_camera.pos());

		m_box_shader.setMat4("model", x_trans);
		Primitives::Render("box");
		m_box_shader.setMat4("model", y_trans);
		Primitives::Render("box");
		m_box_shader.setMat4("model", z_trans);
		Primitives::Render("box");

		Primitives::UnbindVAO();
		projection = glm::perspective(glm::radians(fov), m_screen_width / (float)m_screen_height, 0.01f, 1.0f);
	}
}