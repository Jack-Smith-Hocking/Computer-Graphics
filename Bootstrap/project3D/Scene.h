#pragma once
#include <vector>

class Light;

class Scene
{
public:

	void Update(float deltaTime);

	void AddLight(Light* light);

	std::vector<Light*> m_lights;
};

