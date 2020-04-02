#include "Scene.h"
#include "Light.h"

void Scene::Update(float deltaTime)
{
	for each (Light* light in m_lights)
	{
		light->Update(deltaTime);
	}
}

void Scene::AddLight(Light* light)
{
	if (light != nullptr)
	{
		m_lights.push_back(light);
	}
}
