#pragma once
#include <glm\ext.hpp>

class Light
{
public:

	void Update(float deltaTime);

	glm::vec3 m_colour;
	glm::vec3 m_direction;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
};

