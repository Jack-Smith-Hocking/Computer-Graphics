#pragma once
class Interactive
{
public:

	virtual void Update(float deltaTime) = 0;

	virtual void OnClick() { m_showMenu = true; };
	virtual void OffClick() { m_showMenu = false; };

	float m_radius = 2;
	
	bool m_showMenu = false;
	bool m_debug = false;
};

