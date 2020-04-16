#include "Game.h"
#include "GL\glew.h"
#include "SDL.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include "Misc.h"
#include "Primitives.h"

static auto lightTransform = glm::mat4(1);

glm::mat4 x_trans = glm::mat4(1);
glm::mat4 y_trans = glm::mat4(1);
glm::mat4 z_trans = glm::mat4(1);

auto then = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();


glm::mat4 model;

Game::Game(const size_t screen_width, const size_t screen_height, const std::atomic<bool>* keys, const std::atomic<int>* mouse_delta)
	: m_screen_width(screen_width)
	, m_screen_height(screen_height)
	, m_keys(keys)
	, m_mouse_delta(mouse_delta)
	, m_terrain()
	, m_camera(glm::vec3(0.0f,3.0f, 5.0f), keys, mouse_delta)
	, m_default_shader("shaders/default.vertex.fx", "shaders/default.fragment.fx")
	, m_box_shader("shaders/box.vertex.fx", "shaders/box.fragment.fx")
{
	Primitives::Init();

	x_trans = glm::translate(x_trans, glm::vec3(5, 0, 0));
	y_trans = glm::translate(y_trans, glm::vec3(0, 5, 0));
	z_trans = glm::translate(z_trans, glm::vec3(0, 0, 5));

	x_trans = glm::scale(x_trans, glm::vec3(10, 0.1, 0.1));
	y_trans = glm::scale(y_trans, glm::vec3(0.1, 10, 0.1));
	z_trans = glm::scale(z_trans, glm::vec3(0.1, 0.1, 10));
}

void Game::tick(float dt) {
	m_camera.tick(dt);
	m_terrain.tick();
}

void Game::render() {
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), m_screen_width / (float)m_screen_height, 0.1f, 10000.0f);
	glm::mat4 view = m_camera.lookAt();
	glm::mat4 pv = projection * view;

    m_box_shader.use();
    m_box_shader.setMat4("view_projection", pv);

	Primitives::BindVAO();

    m_box_shader.setMat4("model", x_trans);
	Primitives::Render("box");
	m_box_shader.setMat4("model", y_trans);
	Primitives::Render("box");
	m_box_shader.setMat4("model", z_trans);
	Primitives::Render("box");

	Primitives::UnbindVAO();
	
	m_terrain.render(m_default_shader, pv);
}