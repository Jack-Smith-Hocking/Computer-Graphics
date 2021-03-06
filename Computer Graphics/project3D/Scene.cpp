#include "Scene.h"

#include "Interactable.h"
#include "Light.h"
#include "Model.h"

#include "Gizmos.h"
#include "Camera.h"
#include "ImGuiFunctions.h"
#include <algorithm>
#include <vector>

extern unsigned int g_windowWidth;
extern unsigned int g_windowHeight;

extern glm::vec4 g_selectedColour;

extern int g_gridSize;

extern std::string g_soulSpearDir;
extern std::string g_bunnyDir;
extern std::string g_dragonDir;
extern std::string g_buddhaDir;
extern std::string g_lucyDir;

Scene::Scene(Camera* camera) : m_mainCamera(camera)
{
	if (camera == nullptr)
	{
		printf("The camera is null, this will cause issues!");
	}
	else
	{
		// Tells the Camera that it is operating within this Scene
		camera->SetScene(this);
	}
}

void Scene::Update(float deltaTime)
{
	SafelyCheckToDelete(m_lights); // Check if any Lights are marked for deletion
	SafelyCheckToDelete(m_models); // Check if any Models are marked for deletion

	UpdateImGui();

	m_mainCamera->Update(deltaTime);

	for each (Light* light in m_lights) // Update Lights
	{
		light->Update(deltaTime);
		light->ShowCollider(m_debugInfo.showColliders);
	}
	for each (Interactable * model in m_models) // Update Models
	{
		model->Update(deltaTime);
		model->ShowCollider(m_debugInfo.showColliders);
	}
}

void Scene::Draw()
{
	DrawGrid(g_gridSize, m_gridInfo.drawGrid); // Draw grid

	EvaluateLights();

	for each (Interactable* model in m_models) // Draw Models
	{
		model->Draw(this);
	}

	m_availableLights.clear(); // Clear the available Lights
}

void Scene::SetShaders(aie::ShaderProgram* textured, aie::ShaderProgram* untextured)
{
	m_texturedShader = textured;
	m_untexturedShader = untextured;
}

void Scene::UpdateImGui()
{
	if (!m_debugInfo.hideErrorPanel)
	{
		ImGuiFunctions::DisplayError("Warning! Max number of lights enabled! All new lights will be disabled!", ImVec4(1, 1, 0, 1), m_maxLightsReached);
		ImGuiFunctions::DisplayError(m_currentModelError, ImVec4(1, 0, 0, 1), true);
	}

	ImGui::Begin("Scene Settings");

	ImGui::Text("Issues Panel");
	ImGuiFunctions::OpenSection(1, 1, true, true);

	ImGui::Checkbox("Hide Issues Panel", &m_debugInfo.hideErrorPanel);
	
	if (ImGui::Button("Clear Issues", ImVec2(100, 20)))
	{
		m_currentModelError.clear();
	}

	ImGuiFunctions::CloseSection(1, 1, true, true);

	ImGui::Checkbox("Show Colliders", &m_debugInfo.showColliders);
	ImGui::Checkbox("Show Grid", &m_gridInfo.drawGrid);

	if (m_gridInfo.drawGrid) // Grid settings
	{
		ImGuiFunctions::OpenSection(0, 0, true, false);

		ImGui::SliderFloat3("Grid Colour", &m_gridInfo.colour[0], 0, 1);
		ImGuiFunctions::ButtonSet<glm::vec3>("Reset Grid Colour", m_gridInfo.colour, glm::vec3(0, 0, 0), ImVec2(125, 20));

		ImGui::SliderInt("Grid Size", &g_gridSize, 10, 250);
		ImGuiFunctions::ButtonSet<int>("Reset Grid Size", g_gridSize, 10, ImVec2(125, 20));

		ImGuiFunctions::CloseSection(2, 2, true, true);
	}

	ImGui::SliderFloat4("Selected Colour", &g_selectedColour[0], 0, 1);
	ImGuiFunctions::ButtonSet<glm::vec4>("Reset Selected Colour", g_selectedColour, glm::vec4(1, 0, 0, 0.3f), ImVec2(150, 20));

	if (ImGui::Button("Add Light", ImVec2(100, 20)))
	{
		Light* newLight = new Light(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
		newLight->m_enabled = false;

		AddLight(newLight);
	}

	ImGuiFunctions::ButtonSet<bool>("Load Model", m_loadModelMenuOpen, true, ImVec2(100, 20));

	ImGui::End();

	if (m_loadModelMenuOpen)
	{
		LoadModelMenu();
	}
}

void Scene::LoadModelMenu()
{
	ImGui::Begin("Load Model");

	ImGui::Text("Models");
	ImGuiFunctions::OpenSection(1, 1, true, true);

	static std::string errorMsg;

	if (ImGui::Button("Load Spear", ImVec2(85, 30)))
	{
		LoadModel(g_soulSpearDir, m_texturedShader, errorMsg);
	}

	if (ImGui::Button("Load Bunny", ImVec2(85, 30)))
	{
		LoadModel(g_bunnyDir, m_untexturedShader, errorMsg);
	}

	if (ImGui::Button("Load Dragon", ImVec2(85, 30)))
	{
		LoadModel(g_dragonDir, m_untexturedShader, errorMsg);
	}

	if (ImGui::Button("Load Buddha", ImVec2(85, 30)))
	{
		LoadModel(g_buddhaDir, m_untexturedShader, errorMsg);
	}

	if (ImGui::Button("Load Lucy", ImVec2(85, 30)))
	{
		LoadModel(g_lucyDir, m_untexturedShader, errorMsg);
	}

	ImGuiFunctions::CloseSection(1, 1, true, true);

	ImGuiFunctions::ButtonSet<bool>("Quit", m_loadModelMenuOpen, false, ImVec2(75, 30), false);

	ImGui::End();
}

void Scene::DrawGrid(int halfGridSize, bool doDraw)
{
	if (!doDraw) return;

	// draw a simple grid with gizmos
	glm::vec4 white(1);
	glm::vec4 gridColour = glm::vec4(m_gridInfo.colour, 1);

	glm::vec4 blue(0, 0, 1, 1);
	glm::vec4 red(1, 0, 0, 1);
	glm::vec4 green(0, 1, 0, 1);

	int gridSize = 2 * halfGridSize;

	aie::Gizmos::addLine(glm::vec3(0, -halfGridSize, 0), glm::vec3(0, halfGridSize, 0), green);

	for (int i = 0; i < gridSize + 1; i++)
	{
		aie::Gizmos::addLine(glm::vec3(-halfGridSize + i, 0, halfGridSize), glm::vec3(-halfGridSize + i, 0, -halfGridSize), i == halfGridSize ? blue : gridColour);

		aie::Gizmos::addLine(glm::vec3(halfGridSize, 0, -halfGridSize + i), glm::vec3(-halfGridSize, 0, -halfGridSize + i), i == halfGridSize ? red : gridColour);
	}
}

void Scene::AddLight(Light* light)
{
	if (light != nullptr)
	{
		m_lights.push_back(light);
	}
}

void Scene::AddModel(Model* model)
{
	if (model != nullptr)
	{
		m_models.push_back(model);
	}
}

void Scene::AttemptToDelete(std::vector<Object*>& objList, Object* obj)
{
	if (obj != nullptr)
	{
		objList.erase(std::find(objList.begin(), objList.end(), obj));
	}
}

void Scene::SafelyCheckToDelete(std::vector<Object*>& objs)
{
	std::vector<Object*> objsToDelete;

	// Iterate through list to check if any are marked for deleting
	for each (Object* obj in objs)
	{
		if (obj->ReadyForDeletion())
		{
			objsToDelete.push_back(obj);
		}
	}

	// Gather all the objects marked for deleting then delete them one at a time
	int size = objsToDelete.size();
	for (int i = 0; i < size; i++)
	{
		if (objsToDelete[i] != nullptr)
		{
			AttemptToDelete(objs, objsToDelete[i]);

			objsToDelete[i]->DeleteObject();
		}
	}
}

void Scene::BindShaderUniforms(aie::ShaderProgram* shader)
{
	// Check if a valid shader was given
	if (shader == nullptr)
	{
		printf("Attempted to use a null shader. No action performed.");
		return;
	}

	// Create simple passable arrays
	glm::vec3 pos[8];
	glm::vec3 col[8];

	int numLights = m_availableLights.size();

	float in[8];

	int i = 0;
	// Convert the available lights to primitive arrays
	for (i; i < m_availableLights.size(); i++)
	{
		pos[i] = m_availableLights[i]->m_position;
		col[i] = m_availableLights[i]->m_colour;

		in[i] = m_availableLights[i]->m_intensity;
	}

	shader->bind();

	// Bind relevant information
	shader->bindUniform("LightPosition", numLights, pos);
	shader->bindUniform("LightColour", numLights, col);
	shader->bindUniform("LightIntensity", numLights, in);

	shader->bindUniform("NumLights", numLights);

	shader->bindUniform("CameraPosition", glm::vec3(glm::inverse(m_mainCamera->GetViewMatrix())[3]));
}

bool Scene::LoadModel(std::string fileDir, aie::ShaderProgram* shader, std::string& errorMsg)
{
	bool hasError = false;

	if (shader == nullptr || fileDir.empty())
	{
		hasError = true;
	}

	if (!hasError)
	{
		Model* m = new Model(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), shader);
		hasError = m == nullptr;

		if (!hasError)
		{
			hasError = !m->Load(fileDir.c_str());

			if (!hasError)
			{
				AddModel(m);
			}
		}
	}

	if (hasError) // Add a message to the error log
	{
		errorMsg += "An error occurred loading model at: " + fileDir + "\n";
		m_currentModelError = errorMsg;
	}

	return hasError;
}

void Scene::EvaluateLights()
{
	// Check which Lights will be available, max Lights will be 8 based off of which ones are enabled
	if (m_availableLights.size() == 0)
	{
		Light* light = nullptr;

		for (int i = 0; i < m_lights.size(); i++)
		{
			light = dynamic_cast<Light*>(m_lights[i]);

			if (light != nullptr)
			{
				if (light->m_enabled && m_availableLights.size() < 8)
				{
					m_availableLights.push_back(light);
					m_maxLightsReached = false;
				}
				else if (m_availableLights.size() >= 8)
				{
					light->m_enabled = false;
					m_maxLightsReached = true;
				}
			}
		}
	}
}

glm::mat4 Scene::GetProjectionMatrix()
{
	return m_mainCamera->GetProjectionMatrix(g_windowWidth, g_windowHeight);
}

glm::mat4 Scene::GetViewMatrix()
{
	return m_mainCamera->GetViewMatrix();
}
