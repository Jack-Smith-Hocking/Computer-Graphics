#pragma once
#include <glm\ext.hpp>
#include "Interactive.h"

class Light : public Interactive
{
public:
	Light(glm::vec3 position, glm::vec3 colour);

	void Update(float deltaTime) override;
	void Draw(Scene* scene) override;

	glm::vec3 m_colour;

	bool m_enabled;

	float m_intensity = 1;
};

