#include "Light.h"
#include "Gizmos.h"
#include "imgui.h"
#include "Object.h"
#include "ImGuiFunctions.h"

extern int g_gridSize;

Light::Light(glm::vec3 position, glm::vec3 colour) : Interactable(position), m_colour(colour)
{
	m_radius = 0.5f + m_intensity * 0.01f;
}

void Light::Update(float deltaTime)
{
	if (m_showMenu)
	{
		ImGui::Begin("Light Settings");

		ImGui::Checkbox("Enabled", &m_enabled);
		ImGui::Checkbox("Hide Light", &m_hide);

		ImGuiFunctions::OpenSection(1, 1, true, true);

		ImGui::SliderFloat3("Position", &m_position[0], -g_gridSize, g_gridSize);
		ImGui::SliderFloat3("Colour", &m_colour[0], 0, 1);


		ImGui::SliderFloat("Intensity", &m_intensity, 0, 100);

		if (ImGui::Button("Delete", ImVec2(50, 20)))
		{
			m_attemptDelete = true;
		}

		ImGuiFunctions::CloseSection(1, 1, true, true);

		ImGui::End();
	}

	m_radius = 0.5f + m_intensity * 0.01f;
	
	if (!m_hide)
	{
		// Draw light visualisation
		aie::Gizmos::addSphere(m_position, m_radius, 16, 8, glm::vec4(m_colour, (m_enabled ? 1 : 0.3f)));
	}

	Interactable::Update(deltaTime);
}

void Light::Draw(Scene* scene)
{
}

