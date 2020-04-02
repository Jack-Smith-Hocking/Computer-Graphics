#include "Camera.h"
#include "imgui.h"
#include "Interactive.h"
#include "Application3D.h"
#include "Scene.h"
#include "Gizmos.h"

#include "Light.h"
#include "Model.h"

extern unsigned int g_windowHeight;
extern unsigned int g_windowWidth;

Camera::Camera()
{
	m_input = aie::Input::getInstance();
}

void Camera::Update(float deltaTime)
{
	DrawMenu();

	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	// Calculate the forwards for the right axis and up axis for the camera
	glm::vec3 forward = GetForward();
	glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
	glm::vec3 up(0, 1, 0);

	float moveSpeed = m_moveSpeed * deltaTime;
	float scrollSpeed = m_scrollSpeed * deltaTime;

	float scrollThisFrame = m_input->getMouseScroll() - m_scrollWheelMove;
	m_scrollWheelMove = m_input->getMouseScroll();

	// Use WASD, ZX keys to move camera around
	if (m_input->isKeyDown(aie::INPUT_KEY_X))
	{
		m_position += up * moveSpeed;
	}
	if (m_input->isKeyDown(aie::INPUT_KEY_Z))
	{
		m_position -= up * moveSpeed;
	}
	if (m_input->isKeyDown(aie::INPUT_KEY_W) || scrollThisFrame > 0)
	{
		m_position += forward * (scrollThisFrame > 0 ? scrollSpeed : moveSpeed);
	}
	if (m_input->isKeyDown(aie::INPUT_KEY_S) || scrollThisFrame < 0)
	{
		m_position -= forward * (scrollThisFrame < 0 ? scrollSpeed : moveSpeed);
	}
	if (m_input->isKeyDown(aie::INPUT_KEY_D))
	{
		m_position += right * moveSpeed;
	}
	if (m_input->isKeyDown(aie::INPUT_KEY_A))
	{
		m_position -= right * moveSpeed;
	}

	// Get current mouse coordinates 
	float mx = m_input->getMouseX();
	float my = m_input->getMouseY();

	if (m_input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		SelectTarget();
	}

	// If the right button is down increment theta and phi
	if (m_input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_MIDDLE))
	{
		m_theta += (m_invertHorizontal ? -1 : 1) * (m_turnSpeed * (mx - m_lastMouseX));
		m_phi += (m_invertVertical ? -1 : 1) * (m_turnSpeed * (my - m_lastMouseY));

		m_phi = glm::clamp(m_phi, -70.0f, 70.0f);
	}

	// Store this frame's mouse position for next frame
	m_lastMouseX = mx;
	m_lastMouseY = my;

	// Add Gizmos
	{
		glm::vec2 mousePos = glm::vec2(g_windowWidth / 2, g_windowHeight / 2);

		if (m_debugInfo.showDebug)
		{
			aie::Gizmos::addLine(m_debugInfo.position, m_debugInfo.position + m_debugInfo.forward * 100.0f, m_debugInfo.lineColour);
			if (m_debugInfo.showSphere)
			{
				aie::Gizmos::addSphere(m_position, 1, 16, 8, glm::vec4(1, 1, 1, 0));
			}
		}

		if (m_crosshairInfo.showCrosshair)
		{
			aie::Gizmos::add2DCircle(mousePos, m_crosshairInfo.size, 12, m_crosshairInfo.colour);
		}
	}
}

void Camera::DrawMenu()
{
	ImGui::Begin("Camera Settings");

	ImGui::Checkbox("Show Debug", &m_debugInfo.showDebug);
	if (m_debugInfo.showDebug)
	{
		ImGui::Indent();

		ImGui::Checkbox("Show Wire Cage", &m_debugInfo.showSphere);

		ImGui::SliderFloat3("Line Colour", &m_debugInfo.lineColour[0], 0, 1);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Spacing();
		ImGui::Spacing();
	}

	ImGui::Checkbox("Show Crosshair", &m_crosshairInfo.showCrosshair);
	if (m_crosshairInfo.showCrosshair)
	{
		ImGui::Indent();

		ImGui::SliderFloat("Size", &m_crosshairInfo.size, 0, 20);

		ImGui::SliderFloat3("Crosshair Colour", &m_crosshairInfo.colour[0], 0, 1);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Spacing();
		ImGui::Spacing();
	}

	ImGui::Checkbox("Invert Vertical", &m_invertVertical);
	ImGui::Checkbox("Invert Horizontal", &m_invertHorizontal);

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::SliderFloat("Turn Speed", &m_turnSpeed, 0.0f, 0.5f);
	ImGui::SliderFloat("Move Speed", &m_moveSpeed, 0.0f, 20.0f);
	ImGui::SliderFloat("Scroll Speed", &m_scrollSpeed, 0.0f, 50.0f);
	ImGui::End();
}

glm::vec3 Camera::GetForward()
{
	// Calculate the forward 
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);
	glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));

	return glm::normalize(forward);
}

void Camera::SelectTarget()
{
	if (m_scene != nullptr)
	{
		glm::vec3 dir = GetForward();
		
		m_debugInfo.forward = dir;
		m_debugInfo.position = m_position;

		Interactive* closestInteractive = nullptr;

		// The closes viable light from m_position
		float closestDist = 1000000000;

		closestInteractive = EvaluateTargets(m_scene->m_lights, dir, closestDist);
		Interactive* temp = EvaluateTargets(m_scene->m_models, dir, closestDist);

		if (temp != nullptr)
		{
			closestInteractive = temp;
		}

		if (closestInteractive != nullptr)
		{
			if (m_currentMenu != nullptr)
			{
				m_currentMenu->OffClick();
			}

			m_currentMenu = closestInteractive;
			m_currentMenu->OnClick();
		}
	}
	else
	{
		printf("The scene given to the camera was nullptr, no raycast performed!");
	}
}

Interactive* Camera::EvaluateTargets(const std::vector<Interactive*>& interactables, const glm::vec3& dir, float& closestDist)
{
	// Vector between the ray origin and the centre of the collision sphere
	glm::vec3 L;

	// Closest point #distance that is perpendicular to the centre of the sphere
	float closestDistToCentre = 0;

	float d = 0;

	// Start point of the ray
	glm::vec3 origin = m_position;

	for each (Interactive* interactable in interactables)
	{
		L = interactable->GetOffsetPosition() - m_position;
		closestDistToCentre = glm::dot(L, dir);

		if (closestDistToCentre < 0)
		{
			continue;
		}

		d = glm::dot(L, L) - (closestDistToCentre * closestDistToCentre);
		if (d < 0 || d >(interactable->GetRadius() * interactable->GetRadius()))
		{
			continue;
		}

		float tempDist = glm::distance(interactable->GetOffsetPosition(), m_position);

		if (tempDist < closestDist)
		{
			closestDist = tempDist;

			return interactable;
		}
	}

	return nullptr;
}

void Camera::SetScene(Scene* scene)
{
	if (scene != nullptr)
	{
		m_scene = scene;
	}
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
