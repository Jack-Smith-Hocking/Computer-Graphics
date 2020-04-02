#pragma once
#include "Object.h"

class Interactive : public Object
{
public:

	Interactive() {};
	Interactive(glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 euler = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1));

	virtual void OnClick() { m_showMenu = true; };
	virtual void OffClick() { m_showMenu = false; };

	virtual float GetRadius() { return m_radius; };

	void ToggleCollider(bool showCollider) { m_showCollider = showCollider; };

	glm::vec3 GetOffsetPosition() { return m_position + m_offset; };

protected:
	float m_radius = 2;
	
	glm::vec3 m_offset = glm::vec3(0, 0, 0);

	bool m_showMenu = false;
	bool m_debug = true;
	bool m_showCollider = false;
};

