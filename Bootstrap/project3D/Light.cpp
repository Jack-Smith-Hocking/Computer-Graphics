#include "Light.h"
#include "Gizmos.h"
#include "imgui.h"

extern int g_gridSize;

Light::Light(glm::vec3 position, glm::vec3 colour, float radius) : m_position(position), m_colour(colour)
{
	m_radius = radius;
}

void Light::Update(float deltaTime)
{
	if (m_showMenu)
	{
		ImGui::Begin("Light Settings");
		ImGui::SliderFloat3("Position", &m_position[0], -g_gridSize, g_gridSize);
		ImGui::SliderFloat3("Colour", &m_colour[0], 0, 1);
		ImGui::End();
	}

	// Draw light visualisation
	aie::Gizmos::addSphere(m_position, m_radius, 16, 8, glm::vec4(m_colour, 1));

	if (m_debug)
	{
		// Draw InteractiveMenu range 
		aie::Gizmos::addSphere(m_position, m_radius, 16, 8, glm::vec4(0, 0, 0, 0));
	}
}

