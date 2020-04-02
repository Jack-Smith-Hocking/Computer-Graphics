#pragma once
#include <glm\ext.hpp>
#include "Interactive.h"

class Light : public Interactive
{
public:
	Light(glm::vec3 position, glm::vec3 colour, float radius);

	void Update(float deltaTime) override;

	glm::vec3 m_colour;
	glm::vec3 m_position;

	float m_intensity = 1;
};

