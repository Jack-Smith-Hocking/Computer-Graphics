#pragma once
#include <vector>
#include "Shader.h"

class Light;
class Model;
class Camera;
class Interactive;
class Object;

class Scene
{
public:

	Scene(Camera* camera);

	void Update(float deltaTime);
	void Draw();

	void UpdateImGui();

	void DrawGrid(int gridSize, bool doGrid);

	void AddLight(Light* light);
	void AddModel(Model* model);

	void AttemptToDelete(std::vector<Interactive*>& objList, Object* obj);
	void SafelyCheckToDelete(std::vector<Interactive*>& objs);

	void UseShader(aie::ShaderProgram* shader);

	std::vector<Interactive*> m_models;
	std::vector<Interactive*> m_lights;

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

	glm::vec3 m_ambientLight = glm::vec3(0.25f, 0.25f, 0.25f);

private:
	struct GridInformation
	{
		bool drawGrid = true;
		
		glm::vec3 colour = glm::vec3(0, 0, 0);
	};
	struct DebugInformation
	{
		bool showColliders = true;
	};

	GridInformation m_gridInfo;
	DebugInformation m_debugInfo;

	Camera* m_mainCamera = nullptr;
	std::vector<Light*> m_availableLights; 

	bool m_maxLightsReached = false;
};

