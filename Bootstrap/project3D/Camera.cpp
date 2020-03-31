#include "Camera.h"
#include "Input.h"
#include "imgui.h"

void Camera::Update(float deltaTime)
{
	ImGui::Begin("Camera Settings");
	ImGui::Checkbox("Invert Vertical", &m_invertVertical);
	ImGui::Checkbox("Invert Horizontal", &m_invertHorizontal);
	ImGui::Spacing();
	ImGui::SliderFloat("Turn Speed", &m_turnSpeed, 0.0f, 1.0f);
	ImGui::SliderFloat("Move Speed", &m_moveSpeed, 0.0f, 10.0f);
	ImGui::End();

	aie::Input* input = aie::Input::getInstance();

	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	// Calculate the forwards for the right axis and up axis for the camera
	glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
	glm::vec3 up(0, 1, 0);

	float moveSpeed = deltaTime * m_moveSpeed;

	// Use WASD, ZX keys to move camera around
	if (input->isKeyDown(aie::INPUT_KEY_X))
	{
		m_position += up * moveSpeed;
	}
	if (input->isKeyDown(aie::INPUT_KEY_Z))
	{
		m_position -= up * moveSpeed;
	}
	if (input->isKeyDown(aie::INPUT_KEY_W))
	{
		m_position += forward * moveSpeed;
	}
	if (input->isKeyDown(aie::INPUT_KEY_S))
	{
		m_position -= forward * moveSpeed;
	}
	if (input->isKeyDown(aie::INPUT_KEY_D))
	{
		m_position += right * moveSpeed;
	}
	if (input->isKeyDown(aie::INPUT_KEY_A))
	{
		m_position -= right * moveSpeed;
	}

	// Get current mouse coordinates 
	float mx = input->getMouseX();
	float my = input->getMouseY();

	// If the right button is down increment theta and phi
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		m_theta += (m_invertHorizontal ? -1 : 1) * (m_turnSpeed * (mx - m_lastMouseX));
		m_phi += (m_invertVertical ? -1 : 1) * (m_turnSpeed * (my - m_lastMouseY));

		m_phi = glm::clamp(m_phi, -70.0f, 70.0f);
	}

	// Store this frame's mouse position for next frame
	m_lastMouseX = mx;
	m_lastMouseY = my;
}

glm::mat4 Camera::GetViewMatrix()
{
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	// Calculate forward of the camera
	glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));

	return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix(float w, float h)
{
	return glm::perspective(glm::pi<float>() * 0.25f, w / h, 0.1f, 1000.0f);
}
