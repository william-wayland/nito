#pragma once

#include <atomic>
#include "SDL.h"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public: 
	Camera(const glm::vec3& pos, const std::atomic<bool>* keys, const std::atomic<int>* mouse_delta);
	void setCameraSpeed(float speed) { m_speed = speed; }
	void setCameraSensitivity(float s) { m_rotation_speed = s; }
	void tick(float dt);
	glm::mat4 lookAt();

	glm::vec3& pos() { return m_pos; }

private:
	glm::vec3 m_pos;
	glm::vec3 m_front;
	glm::vec3 m_up;

	const std::atomic<bool>* m_keys;
	const std::atomic<int>* m_mouse_delta;

	float m_speed = 5.0f;
	float m_rotation_speed = 5.0f;
	float m_yaw = -90, m_pitch = 0;
};

