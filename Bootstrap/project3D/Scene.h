#pragma once
#include <vector>

class Light;

class Scene
{
public:

	void AddLight(Light* light);

	std::vector<Light*> m_lights;
};

