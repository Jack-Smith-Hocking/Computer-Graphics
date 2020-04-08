#pragma once
#include "Object.h"
#include <string>

class Interactable : public Object
{
public:

	Interactable() {};
	Interactable(glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 euler = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1));

	virtual void Update(float deltaTime) override;
	virtual void UpdateImGui() {};

	virtual void OnClick() { m_showMenu = true; m_selected = true; };		// When selected
	virtual void OffClick() { m_showMenu = false; m_selected = false; };	// When unselected

	virtual float GetRadius() { return m_radius; };

	void ShowCollider(bool showCollider) { m_showCollider = showCollider; };

	glm::vec3 GetOffsetPosition() { return m_position + m_offset; };		// Position + m_offset

	virtual void DeleteObject() override { Object::DeleteObject(); };

protected:

	float m_radius = 2;								// How big the collision radius will be for raycast detection
	
	glm::vec3 m_offset = glm::vec3(0, 0, 0);		// The offset of the sphere collider from the base position

	bool m_showMenu = false;						// Whether or not to show the ImGui interface
	bool m_showCollider = false;					// Whether or not to display the sphere collider
	bool m_selected = false;						// Whether this Interactable is selected or not

	bool m_debug = true;							// Whether or not to show debug information
};

