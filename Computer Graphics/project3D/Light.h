#pragma once
#include <glm\ext.hpp>
#include "Interactable.h"

class Light : public Interactable
{
public:
	Light(glm::vec3 position, glm::vec3 colour);

	void Update(float deltaTime) override;
	void UpdateImGui() override;

	glm::vec3 m_colour;				// Colour of the Light 
	
	bool m_enabled = true;			// Whether or not the Light is currently lighting the scene

	float m_intensity = 1;			// How bright a Light will be
};

