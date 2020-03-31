#pragma once
#include <glm\ext.hpp>
#include "Input.h"

class InteractiveMenu;
class Scene;

struct RayInfo
{
	glm::vec3 direction;
	glm::vec3 start;
	glm::vec3 end;

	glm::vec2 mousePos;
};

class Camera
{
public:

	Camera(Scene* scene);

	void Update(float deltaTime);

	RayInfo ProjectRay(float maxDistance, bool debug = false);
	void SelectTarget();

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float w, float h);

private:
	float m_theta = 215;
	float m_phi = -25;

	float m_lastMouseX = 0;
	float m_lastMouseY = 0;

	float m_turnSpeed = 0.1f;
	float m_moveSpeed = 7.5f;

	bool m_invertVertical = false;
	bool m_invertHorizontal = false;

	aie::Input* m_input;

	glm::vec3 m_position = glm::vec3(10, 10, 10);

	Scene* m_scene = nullptr;
	InteractiveMenu* m_currentMenu = nullptr;

	RayInfo m_currentRay;
};

