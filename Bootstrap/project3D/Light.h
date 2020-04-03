#pragma once
#include <glm\ext.hpp>
#include "Interactable.h"

class Light : public Interactable
{
public:
	Light(glm::vec3 position, glm::vec3 colour);

	void Update(float deltaTime) override;
	void Draw(Scene* scene) override;

	glm::vec3 m_colour;

	bool m_enabled = true;

	float m_intensity = 1;
};

