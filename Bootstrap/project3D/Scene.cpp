#include "Scene.h"
#include "Light.h"

void Scene::AddLight(Light* light)
{
	if (light != nullptr)
	{
		m_lights.push_back(light);
	}
}
