#include "Scene.h"

#include "Interactive.h"
#include "Light.h"
#include "Model.h"

#include "Gizmos.h"
#include "imgui.h"
#include "Camera.h"
#include <algorithm>
#include <vector>

extern unsigned int g_windowWidth;
extern unsigned int g_windowHeight;

extern int g_gridSize;

Scene::Scene(Camera* camera) : m_mainCamera(camera)
{
	if (camera == nullptr)
	{
		printf("The camera is null, this will cause issues!");
	}
	else
	{
		camera->SetScene(this);
	}
}

void Scene::Update(float deltaTime)
{
	UpdateImGui();

	m_mainCamera->Update(deltaTime);

	for each (Light* light in m_lights)
	{
		light->Update(deltaTime);
		light->ToggleCollider(m_debugInfo.showColliders);
	}
	for each (Interactive * model in m_models)
	{
		model->Update(deltaTime);
		model->ToggleCollider(m_debugInfo.showColliders);
	}
}

void Scene::Draw()
{
	DrawGrid(g_gridSize, m_gridInfo.drawGrid);

	for each (Interactive* model in m_models)
	{
		model->Draw(this);
	}

	m_availableLights.clear();
}

void Scene::UpdateImGui()
{
	SafelyCheckToDelete(m_lights);
	SafelyCheckToDelete(m_models);

	if (m_maxLightsReached)
	{
		ImGui::BeginMainMenuBar();
		
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning! Max number of lights enabled!");

		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("Scene Settings");
	ImGui::Checkbox("Show Colliders", &m_debugInfo.showColliders);
	ImGui::Checkbox("Show Grid", &m_gridInfo.drawGrid);

	if (m_gridInfo.drawGrid)
	{
		ImGui::Indent();

		ImGui::SliderFloat3("Grid Colour", &m_gridInfo.colour[0], 0, 1);

		ImGui::SliderInt("Grid Size", &g_gridSize, 10, 250);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Spacing();
		ImGui::Spacing();
	}

	if (ImGui::Button("Add Light", ImVec2(100, 40)))
	{
		Light* newLight = new Light(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
		newLight->m_enabled = false;

		AddLight(newLight);
	}

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

void Scene::AttemptToDelete(std::vector<Interactive*>& objList, Object* obj)
{
	if (obj != nullptr)
	{
		objList.erase(std::find(objList.begin(), objList.end(), obj));
	}
}

void Scene::SafelyCheckToDelete(std::vector<Interactive*>& objs)
{
	std::vector <Object* > objsToDelete;

	// Iterate through list to check if any are marked for deleting
	for each (Object* obj in objs)
	{
		if (obj->m_attemptDelete)
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
		}
	}
}

void Scene::UseShader(aie::ShaderProgram* shader)
{
	// Check if a valid shader was given
	if (shader == nullptr)
	{
		printf("Attempted to use a null shader. No action performed.");
		return;
	}

	// Check which lights will be available, max lights will be 8 based off of which ones are enabled
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

	// Create simple passable arrays
	glm::vec3 pos[8];
	glm::vec3 col[8];

	float in[8];

	int i = 0;
	// Convert the available lights to primitive arrays
	for (i; i < m_availableLights.size(); i++)
	{
		pos[i] = m_availableLights[i]->m_position;
		col[i] = m_availableLights[i]->m_colour;

		in[i] = m_availableLights[i]->m_intensity;
	}
	for (i; i < 8; i++)
	{
		pos[i] = glm::vec3(0, 0, 0);
		col[i] = glm::vec3(0, 0, 0);

		in[i] = 0;
	}

	shader->bind();

	// Bind relevant information
	shader->bindUniform("LightPosition", 8, pos);
	shader->bindUniform("LightColour", 8, col);

	shader->bindUniform("LightIntensity", 8, in);

	shader->bindUniform("CameraPosition", glm::vec3(glm::inverse(m_mainCamera->GetViewMatrix())[3]));

	shader->bindUniform("Ia", m_ambientLight);
}

glm::mat4 Scene::GetProjectionMatrix()
{
	return m_mainCamera->GetProjectionMatrix(g_windowWidth, g_windowHeight);
}

glm::mat4 Scene::GetViewMatrix()
{
	return m_mainCamera->GetViewMatrix();
}
