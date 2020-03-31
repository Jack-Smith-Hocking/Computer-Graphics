#include "Light.h"
#include "Gizmos.h"
#include "imgui.h"

void Light::Update(float deltaTime)
{
	ImGui::Begin("Light Settings");
	ImGui::SliderFloat3("Direction", &m_direction[0], -2 * glm::pi<float>(), 2 * glm::pi<float>());
	ImGui::End();

	aie::Gizmos::addLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0) - glm::normalize(m_direction) * 10.0f, glm::vec4(m_colour, 1));
	aie::Gizmos::addSphere(glm::vec3(0, 0, 0) - glm::normalize(m_direction) * 10.0f, 1, 16, 8, glm::vec4(0, 0, 0, 1));
}
