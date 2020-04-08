#pragma once
#include "Shader.h"
#include "imgui.h"
#include <vector>
#include <string>

class Light;
class Model;
class Camera;
class Interactable;
class Object;

class Scene
{
public:

	Scene(Camera* camera);

	void Update(float deltaTime);
	void Draw();

	// Set default textured and untextured shaders for Model loading
	void SetShaders(aie::ShaderProgram* textured, aie::ShaderProgram* untextured);

	void UpdateImGui();
	void LoadModelMenu();

	// Draws the grid
	void DrawGrid(int gridSize, bool doGrid);

	void AddLight(Light* light);
	void AddModel(Model* model);

	// Try and safely erase elements of the m_models or m_lights lists
	void AttemptToDelete(std::vector<Object*>& objList, Object* obj);
	// Check if any Objects are marked for deletion
	void SafelyCheckToDelete(std::vector<Object*>& objs);

	// Bind all the general shader uniforms of a Model
	void BindShaderUniforms(aie::ShaderProgram* shader);

	// Load a Model into the scene
	bool LoadModel(std::string fileDir, aie::ShaderProgram* shader, std::string& errorMsg);

	// Check which Lights are available for lighting
	void EvaluateLights();

	std::vector<Object*> m_models;
	std::vector<Object*> m_lights;

	// Returns the Main Camera's Projection Matrix
	glm::mat4 GetProjectionMatrix();
	// Returns the Main Camera's View Matrix
	glm::mat4 GetViewMatrix();

private:

	struct GridInformation
	{
		bool drawGrid = true;
		
		glm::vec3 colour = glm::vec3(0, 0, 0);
	};
	struct DebugInformation
	{
		bool showColliders = true;
		bool hideErrorPanel = false;
	};

	GridInformation m_gridInfo;									// Grid info
	DebugInformation m_debugInfo;								// Debug info

	Camera* m_mainCamera = nullptr;								// Main Camera of the scene
	
	aie::ShaderProgram* m_texturedShader = nullptr;				// Default textured shader for loading new Models
	aie::ShaderProgram* m_untexturedShader = nullptr;			// Default untextured shader for loading new Models

	std::vector<Light*> m_availableLights;						// The currently used Lights for Scene lighting

	std::string m_currentModelError = "";						// Error message for Model loading

	bool m_maxLightsReached = false;							// Whether the Scene has reached maximum active lights
	bool m_loadModelMenuOpen = false;							// Whether the "Load Model" menu is currently open
};

