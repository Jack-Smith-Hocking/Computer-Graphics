#pragma once
#include "Object.h"
#include <string>

class Interactable : public Object
{
public:

	Interactable() {};
	Interactable(glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 euler = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1));

	virtual void Update(float deltaTime) override;

	virtual void OnClick() { m_showMenu = true; m_selected = true; };
	virtual void OffClick() { m_showMenu = false; m_selected = false; };

	virtual float GetRadius() { return m_radius; };

	void ToggleCollider(bool showCollider) { m_showCollider = showCollider; };

	glm::vec3 GetOffsetPosition() { return m_position + m_offset; };

protected:

	static void ImGuiResetFormatting(bool open);

	float m_radius = 2;
	
	glm::vec3 m_offset = glm::vec3(0, 0, 0);

	bool m_showMenu = false;
	bool m_showCollider = false;
	bool m_selected = false;

	bool m_debug = true;
};

