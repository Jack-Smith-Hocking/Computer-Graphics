#pragma once
class InteractiveMenu
{
public:

	virtual void Update(float deltaTime) = 0;

	float m_radius = 2;
	
	bool m_showMenu = true;
	bool m_debug = false;
};

