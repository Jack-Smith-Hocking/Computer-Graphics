#pragma once
#include <glm\ext.hpp>

class Camera
{
public:

	void Update(float deltaTime);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float w, float h);

private:
	float m_theta = 0;
	float m_phi = 0;

	float m_lastMouseX = 0;
	float m_lastMouseY = 0;

	float m_turnSpeed = 0.1f;
	float m_moveSpeed = 1;

	bool m_invertVertical = false;
	bool m_invertHorizontal = false;

	glm::vec3 m_position = glm::vec3(10, 10, 10);
};

