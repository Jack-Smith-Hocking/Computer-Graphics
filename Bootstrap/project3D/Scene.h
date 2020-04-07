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

	void SetShaders(aie::ShaderProgram* textured, aie::ShaderProgram* untextured);

	void UpdateImGui();
	void LoadModelMenu();

	void DrawGrid(int gridSize, bool doGrid);

	void AddLight(Light* light);
	void AddModel(Model* model);

	void AttemptToDelete(std::vector<Object*>& objList, Object* obj);
	void SafelyCheckToDelete(std::vector<Object*>& objs);

	void BindShaderUniforms(aie::ShaderProgram* shader);

	bool LoadModel(std::string fileDir, aie::ShaderProgram* shader, std::string& errorMsg);

	void EvaluateLights();

	std::vector<Object*> m_models;
	std::vector<Object*> m_lights;

	glm::mat4 GetProjectionMatrix();
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

	GridInformation m_gridInfo;
	DebugInformation m_debugInfo;

	Camera* m_mainCamera = nullptr;
	
	aie::ShaderProgram* m_texturedShader = nullptr;
	aie::ShaderProgram* m_untexturedShader = nullptr;

	std::vector<Light*> m_availableLights; 

	std::string m_currentModelError = "";

	bool m_maxLightsReached = false;
	bool m_loadModelMenuOpen = false;
};

