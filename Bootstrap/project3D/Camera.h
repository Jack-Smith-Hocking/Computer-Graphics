#pragma once
#include <glm\ext.hpp>
#include "Input.h"

class Interactable;
class Scene;

class Camera
{
public:

	struct DebugInformation
	{
		bool showDebug = true;
		bool showSphere = true;

		glm::vec3 position;
		glm::vec3 forward;
		
		glm::vec4 lineColour = glm::vec4(1, 1, 1, 1);
	};
	struct CrosshairInformation
	{
		bool showCrosshair = true;

		float size = 10;

		glm::vec4 colour = glm::vec4(1, 1, 1, 1);
	};

	Camera();

	void Update(float deltaTime);

	void DrawMenu();

	glm::vec3 GetForward();
	void SelectTarget();
	Interactable* EvaluateTargets(const std::vector<Interactable*>& interactables, const glm::vec3& dir, float& closestDist);

	void SetScene(Scene* scene);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float w, float h);

private:
	float m_theta = 215;
	float m_phi = -25;

	float m_lastMouseX = 0;
	float m_lastMouseY = 0;

	float m_turnSpeed = 0.1f;
	float m_moveSpeed = 7.5f;

	float m_scrollSpeed = 5.0F;
	float m_scrollWheelMove = 0;

	bool m_invertVertical = false;
	bool m_invertHorizontal = false;

	aie::Input* m_input;

	glm::vec3 m_position = glm::vec3(10, 10, 10);

	DebugInformation m_debugInfo;
	CrosshairInformation m_crosshairInfo;

	Scene* m_scene = nullptr;
	Interactable* m_currentMenu = nullptr;
};

