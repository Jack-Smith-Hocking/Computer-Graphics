#include "Camera.h"
#include "imgui.h"
#include "Interactable.h"
#include "Application3D.h"
#include "Scene.h"
#include "Gizmos.h"
#include "ImGuiFunctions.h"

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
	// If the currently selected Interactable is marked for deletion, then clear it
	if (m_currentMenu != nullptr && m_currentMenu->ReadyForDeletion())
	{
		m_currentMenu = nullptr;
	}

	UpdateImGui();

	// Convert to radians
	float thetaR = glm::radians(m_theta); 
	float phiR = glm::radians(m_phi);

	// Calculate the forwards for the right axis and up axis for the camera
	glm::vec3 forward = GetForward();
	glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
	glm::vec3 up(0, 1, 0);

	float moveSpeed = m_moveSpeed * deltaTime;
	float scrollSpeed = m_scrollSpeed * deltaTime;

	// The amount the user scrolled by this frame, calculated by the total scroll - the scroll from last frame
	// Because aie::Input accumulates scroll over frames rather than clearing, which means it isn't 0 when stopped scrolling
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
			glm::vec2 mousePos = glm::vec2(g_windowWidth / 2, g_windowHeight / 2);

			aie::Gizmos::add2DCircle(mousePos, m_crosshairInfo.size, 12, m_crosshairInfo.colour);
		}
	}
}

void Camera::UpdateImGui()
{
	ImGui::Begin("Camera Settings");

	ImGui::Checkbox("Show Debug", &m_debugInfo.showDebug);
	if (m_debugInfo.showDebug)
	{
		ImGuiFunctions::OpenSection(0, 0, true, false);

		ImGui::Checkbox("Show Wire Cage", &m_debugInfo.showSphere);

		ImGui::SliderFloat3("Line Colour", &m_debugInfo.lineColour[0], 0, 1);

		ImGuiFunctions::CloseSection(2, 2, true, true);
	}

	ImGui::Checkbox("Show Crosshair", &m_crosshairInfo.showCrosshair);
	if (m_crosshairInfo.showCrosshair)
	{
		ImGuiFunctions::OpenSection(0, 0, true, false);

		ImGui::SliderFloat("Size", &m_crosshairInfo.size, 0, 20);

		ImGui::SliderFloat3("Crosshair Colour", &m_crosshairInfo.colour[0], 0, 1);

		ImGuiFunctions::CloseSection(2, 2, true, true);
	}

	ImGui::Checkbox("Invert Vertical", &m_invertVertical);
	ImGui::Checkbox("Invert Horizontal", &m_invertHorizontal);

	ImGuiFunctions::AddSpacings(2);

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
	// Check if this Camera is being used in a Scene
	if (m_scene != nullptr)
	{
		// if there is a currently selected Interactable, tell it that it has been clicked off of
		if (m_currentMenu != nullptr)
		{
			m_currentMenu->OffClick();
			m_currentMenu = nullptr;
		}

		// Get the Camera's forward. This is where we will be the direction we ray cast
		glm::vec3 dir = GetForward();
		
		m_debugInfo.forward = dir;
		m_debugInfo.position = m_position;

		// The closes viable Interactable from m_position
		Interactable* closestInteractive = nullptr;
		float closestDist = 1000000000;

		// Check which Interactables will be hit, and which will be the one affected
		closestInteractive = EvaluateTargets(m_scene->m_lights, dir, closestDist);
		Interactable* temp = EvaluateTargets(m_scene->m_models, dir, closestDist);

		if (temp != nullptr)
		{
			closestInteractive = temp;
		}

		// If an Interactable was selected, tell it that it was clicked
		if (closestInteractive != nullptr)
		{
			m_currentMenu = closestInteractive;
			m_currentMenu->OnClick();
		}
	}
	else
	{
		printf("The scene given to the camera was nullptr, no raycast performed!");
	}
}

Interactable* Camera::EvaluateTargets(const std::vector<Object*>& interactables, const glm::vec3& dir, float& closestDist)
{
	// Vector between the ray origin and the centre of the collision sphere
	glm::vec3 L;

	// Closest point #distance that is perpendicular to the centre of the sphere
	float closestDistToCentre = 0;

	float d = 0;

	// Start point of the ray
	glm::vec3 origin = m_position;

	Interactable* interactable = nullptr;

	// This will explain it better than i would be able to:
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection

	for each (Object* obj in interactables)
	{
		interactable = dynamic_cast<Interactable*>(obj); // Cast to an Interactable
		if (interactable == nullptr)
		{
			continue;
		}

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
	// Calculate forward of the camera
	glm::vec3 forward = GetForward();

	return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix(float w, float h)
{
	return glm::perspective(glm::pi<float>() * 0.25f, w / h, 0.1f, 1000.0f);
}
