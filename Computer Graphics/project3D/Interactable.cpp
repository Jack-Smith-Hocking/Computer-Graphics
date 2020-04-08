#include "Interactable.h"
#include "Gizmos.h"

extern glm::vec4 g_selectedColour;

Interactable::Interactable(glm::vec3 pos, glm::vec3 euler, glm::vec3 scale) : Object(pos, euler, scale)
{
}

void Interactable::Update(float deltaTime)
{
	Object::Update(deltaTime); // Call base Update
	UpdateImGui(); // Draw ImGui interface 
	
	if (m_selected) // Draw an indicator that this Interactable is selected
	{
		aie::Gizmos::addSphere(m_position + m_offset, m_radius * 1.2f, 16, 8, g_selectedColour);
	}
	if (m_showCollider) // Draw the collider
	{
		aie::Gizmos::addSphere(m_position + m_offset, m_radius, 16, 8, glm::vec4(1, 1, 1, 0));
	}
}
