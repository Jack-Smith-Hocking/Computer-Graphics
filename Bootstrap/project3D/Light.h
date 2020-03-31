#pragma once
#include <glm\ext.hpp>
#include "InteractiveMenu.h"

class Light : public InteractiveMenu
{
public:
	Light(glm::vec3 position, glm::vec3 colour, float radius);

	void Update(float deltaTime) override;

	glm::vec3 m_colour;
	glm::vec3 m_position;
};

