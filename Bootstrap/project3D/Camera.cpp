#include "Camera.h"
#include "imgui.h"
#include "InteractiveMenu.h"
#include "Application3D.h"
#include "Scene.h"
#include "Light.h"
#include "Gizmos.h"

extern unsigned int WindowHeight;
extern unsigned int WindowWidth;

Camera::Camera(Scene* scene) : m_scene(scene)
{
	m_input = aie::Input::getInstance();
}

void Camera::Update(float deltaTime)
{
	ImGui::Begin("Camera Settings");
	ImGui::Checkbox("Invert Vertical", &m_invertVertical);
	ImGui::Checkbox("Invert Horizontal", &m_invertHorizontal);
	ImGui::Spacing();
	ImGui::SliderFloat("Turn Speed", &m_turnSpeed, 0.0f, 1.0f);
	ImGui::SliderFloat("Move Speed", &m_moveSpeed, 0.0f, 10.0f);
	ImGui::End();

	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	// Calculate the forwards for the right axis and up axis for the camera
	glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
	glm::vec3 up(0, 1, 0);

	float moveSpeed = deltaTime * m_moveSpeed;

	// Use WASD, ZX keys to move camera around
	if (m_input->isKeyDown(aie::INPUT_KEY_X))
	{
		m_position += up * moveSpeed;
	}
	if (m_input->isKeyDown(aie::INPUT_KEY_Z))
	{
		m_position -= up * moveSpeed;
	}
	if (m_input->isKeyDown(aie::INPUT_KEY_W))
	{
		m_position += forward * moveSpeed;
	}
	if (m_input->isKeyDown(aie::INPUT_KEY_S))
	{
		m_position -= forward * moveSpeed;
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

	if (m_input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		SelectTarget();
	}

	// If the right button is down increment theta and phi
	if (m_input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		m_theta += (m_invertHorizontal ? -1 : 1) * (m_turnSpeed * (mx - m_lastMouseX));
		m_phi += (m_invertVertical ? -1 : 1) * (m_turnSpeed * (my - m_lastMouseY));

		m_phi = glm::clamp(m_phi, -70.0f, 70.0f);
	}

	// Store this frame's mouse position for next frame
	m_lastMouseX = mx;
	m_lastMouseY = my;

	aie::Gizmos::addLine(m_currentRay.start, m_currentRay.end, glm::vec4(1, 1, 1, 1));
	aie::Gizmos::addSphere(m_currentRay.start, 1, 16, 8, glm::vec4(1, 1, 1, 0));

	aie::Gizmos::add2DCircle(m_currentRay.mousePos, 10, 12, glm::vec4(1, 1, 1, 1));

	aie::Gizmos::addSphere(m_position, 1, 16, 8, glm::vec4(1, 1, 1, 1));

	aie::Gizmos::add2DCircle(glm::vec2(0, 0), 4, 12, glm::vec4(1, 1, 1, 1));
}

RayInfo Camera::ProjectRay(float maxDistance, bool debug)
{
	// Get mouse point
	float mx = (2 * m_input->getMouseX() / WindowWidth) - 1.0f;
	float my = (2 * m_input->getMouseY() / WindowHeight) - 1.0f;

	//glm::mat4 invVP = glm::inverse(GetProjectionMatrix(WindowWidth, WindowHeight) * GetViewMatrix());

	//glm::vec4 screenPos = glm::vec4(mx, -my, 1.0f, 1.0f);
	//glm::vec4 worldPos = invVP * screenPos;

	//glm::vec3 dir = glm::vec3(worldPos);

	glm::vec4 ray_clip(mx,  my, -1.0f, 1.0f);
	glm::vec4 ray_eye = glm::inverse(GetProjectionMatrix(WindowWidth, WindowHeight)) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

	glm::vec3 ray_world = glm::vec3(glm::inverse(GetViewMatrix()) * ray_eye);
	ray_world = glm::normalize(ray_world);

	RayInfo rayInfo;
	rayInfo.direction = ray_world;
	rayInfo.start = m_position;
	rayInfo.end = m_position + ray_world * maxDistance;
	//rayInfo.mousePos = glm::vec2(m_input->getMouseX(), m_input->getMouseY());
	rayInfo.mousePos = glm::vec2(mx, my);

	m_currentRay = rayInfo;

	return rayInfo;
}

void Camera::SelectTarget()
{
	if (m_scene != nullptr)
	{
		RayInfo rayInfo = ProjectRay(100, true);

		if (m_currentMenu != nullptr)
		{
			m_currentMenu->m_showMenu = false;
			m_currentMenu = nullptr;
		}

		InteractiveMenu* closestMenu = nullptr;
		float closestDist = 1000000000;

		// Vector between the ray origin and the centre of the collision sphere
		glm::vec3 L;

		// Closest distance that is perpendicular to the centre of the sphere
		float closestDistToCentre = 0;

		float d = 0;

		// Direction the ray is travelling
		glm::vec3 dir = rayInfo.direction;

		// Start point of the ray
		glm::vec3 origin = rayInfo.start;

		for each (Light* l in m_scene->m_lights)
		{
			L = l->m_position - dir;
			closestDistToCentre = glm::dot(L, dir);

			if (closestDistToCentre < 0)
			{
				continue;
			}

			d = glm::sqrt(glm::dot(L, L) - glm::dot(dir, dir));
			if (d < 0 || d > l->m_radius)
			{
				continue;
			}

			if (d < closestDist)
			{
				closestDist = d;
				closestMenu = l;
			}
		}

		if (closestMenu != nullptr)
		{
			m_currentMenu = closestMenu;
			m_currentMenu->m_showMenu = true;
		}
	}
	else
	{
		printf("The scene given to the camera was nullptr, no raycast performed!");
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
