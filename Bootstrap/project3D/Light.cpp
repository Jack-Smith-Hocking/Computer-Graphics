#include "Light.h"
#include "Gizmos.h"
#include "imgui.h"
#include "Object.h"

extern int g_gridSize;

Light::Light(glm::vec3 position, glm::vec3 colour) : Interactive(position), m_colour(colour)
{
	m_radius = 0.5f + m_intensity * 0.01f;
}

void Light::Update(float deltaTime)
{
	if (m_showMenu)
	{
		ImGui::Begin("Light Settings");

		ImGui::Checkbox("Enabled", &m_enabled);

		ImGui::Separator();

		ImGui::SliderFloat3("Position", &m_position[0], -g_gridSize, g_gridSize);
		ImGui::SliderFloat3("Colour", &m_colour[0], 0, 1);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::SliderFloat("Intensity", &m_intensity, 0, 100);

		if (ImGui::Button("Delete", ImVec2(50, 20)))
		{
			m_attemptDelete = true;
		}

		ImGui::End();
	}

	m_radius = 0.5f + m_intensity * 0.01f;
	
	// Draw light visualisation
	aie::Gizmos::addSphere(m_position, m_radius, 16, 8, glm::vec4(m_colour, (m_enabled ? 1 : 0.5f)));
}

void Light::Draw(Scene* scene)
{
}

