#pragma once
#include <glm\ext.hpp>
#include "Input.h"

class Interactable;
class Scene;
class Object;

class Camera
{
public:

	// Small struct used to wrap debug settings nicely
	struct DebugInformation
	{
		bool showDebug = false;
		bool showSphere = true;

		glm::vec3 position = glm::vec3(0 ,0, 0);
		glm::vec3 forward = glm::vec3(0, 0, 0);
		
		glm::vec4 lineColour = glm::vec4(1, 1, 1, 1);
	};
	
	// Small struct used to wrap crosshair settings nicely
	struct CrosshairInformation
	{
		bool showCrosshair = true;

		float size = 10;

		glm::vec4 colour = glm::vec4(1, 1, 1, 1);
	};

	Camera();

	void Update(float deltaTime);

	void UpdateImGui();

	// Calculate the Camera's forward 
	glm::vec3 GetForward();
	// Project a ray into the world and based on the results get the selected Interactable object
	void SelectTarget();
	// Check whether a ray travelling in direction 'dir' will collide with objects in 'interactables'
	Interactable* EvaluateTargets(const std::vector<Object*>& interactables, const glm::vec3& dir, float& closestDist);

	void SetScene(Scene* scene);
	
	glm::mat4 GetViewMatrix();																								
	glm::mat4 GetProjectionMatrix(float w, float h);																		

private:

	float m_theta = 215;							// Camera's horizontal rotation
	float m_phi = -25;								// Camera's vertical rotation
	
	float m_lastMouseX = 0;							// X position of the mouse last frame
	float m_lastMouseY = 0;							// Y position of the mouse last frame

	float m_turnSpeed = 0.1f;						// How fast the Camera will rotate
	float m_moveSpeed = 7.5f;						// How fast the Camera will move through space

	float m_scrollSpeed = 5.0F;						// How fast the Camera can be 'zoomed in'
	float m_scrollWheelMove = 0;					// Last frame's total amount the scroll wheel has been moved

	
	bool m_invertVertical = false;					// Invert vertical rotation
	bool m_invertHorizontal = false;				// Invert horizontal rotation

	aie::Input* m_input;							// Input instance

	glm::vec3 m_position = glm::vec3(10, 10, 10);	// Position of the Camera

	DebugInformation m_debugInfo;					// Debug information
	CrosshairInformation m_crosshairInfo;			// Crosshair information

	Scene* m_scene = nullptr;						// The Scene managing this Camera
	Interactable* m_currentMenu = nullptr;			// The currently selected Interactable object
};

