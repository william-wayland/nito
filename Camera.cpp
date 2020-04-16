#include "Camera.h"

Camera::Camera(const glm::vec3& pos, const std::atomic<bool>* keys, const std::atomic<int>* mouse_delta)
	: m_pos(pos)
	, m_front(glm::vec3(0.0f, 0.0f, -1.0f))
	, m_up(glm::vec3(0.0f, 1.0f, 0.0f))
	, m_keys(keys)
	, m_mouse_delta(mouse_delta)
{
}

void Camera::tick(float dt)
{
	auto speed = m_speed * dt;

	if (m_keys[SDLK_a]) m_pos -= speed * glm::normalize(glm::cross(m_front, m_up));
	if (m_keys[SDLK_d]) m_pos += speed * glm::normalize(glm::cross(m_front, m_up));

	if (m_keys[SDLK_s]) m_pos -= speed * m_front;
	if (m_keys[SDLK_w]) m_pos += speed * m_front;

	if (m_keys[SDLK_q]) m_pos -= speed * m_up;
	if (m_keys[SDLK_e]) m_pos += speed * m_up;

	m_yaw += m_mouse_delta[0] * m_rotationSpeed * dt;
	m_pitch += m_mouse_delta[1] * m_rotationSpeed * dt;
	
	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	direction.y = sin(-glm::radians(m_pitch));
	direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(direction);
}

glm::mat4 Camera::lookAt()
{
	return glm::lookAt(m_pos, m_pos + m_front, m_up);
}
