#pragma once
#include <array>
#include <atomic>

#include "Terrain.h"
#include "Camera.h"
#include "Shader.h"

#include "Definitions.h"

class Game
{
public:
	Game(const size_t screen_width, const size_t screen_height, const std::atomic<bool>* keys, const std::atomic<int>* mouse_delta);
	void tick(float dt);
	void render(bool);

private:

	Terrain m_terrain;
	Terrain m_flat_land;
	Camera m_camera;
	float m_camera_speed;

	Shader m_default_shader;
	Shader m_box_shader;
	Shader m_coloured_box_shader;
	Shader m_model_shader;

	int m_point_lights;

	const std::atomic<bool>* m_keys;
	const std::atomic<int>* m_mouse_delta;
	const size_t m_screen_width;
	const size_t m_screen_height;
};

