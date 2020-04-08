#include "Model.h"
#include "Scene.h"
#include "imgui.h"
#include "Gizmos.h"
#include "ImGuiFunctions.h"

extern int g_gridSize;

Model::Model(glm::vec3 pos, glm::vec3 euler, glm::vec3 scale, aie::ShaderProgram* shader) : Interactable(pos, euler, scale), m_shader(shader)
{

}

Model::~Model()
{
	if (m_mesh != nullptr)
	{
		delete m_mesh;
	}
}

void Model::Draw(Scene* scene)
{
	// Whether or not to show the model
	if (m_hide) return;
	
	// Get the managing scene to bind general uniforms
	if (scene != nullptr)
	{
		scene->BindShaderUniforms(m_shader);
	}

	// Calculate the ProjectionViewModel for this individual Model
	auto pvm = scene->GetProjectionMatrix() * scene->GetViewMatrix() * m_transform;

	// Bind all Model specific uniforms
	m_shader->bindUniform("ProjectionViewModel", pvm);
	m_shader->bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_transform)));
	m_shader->bindUniform("ModelMatrix", m_transform);

	// Draw the Model if it has a mesh
	if (m_mesh != nullptr)
	{
		m_mesh->draw();
	}
}

void Model::UpdateImGui()
{
	// Update the ImGui menu
	if (m_showMenu)
	{
		ImVec2 resetSize = ImVec2(100, 20);

		ImGui::Begin("Model Settings");

		ImGui::Checkbox("Hide Model", &m_hide);

		ImGuiFunctions::OpenSection(1, 1, true, true);

		// Collider Settings
		{
			ImGui::Text("Collider");

			ImGuiFunctions::OpenSection(0, 0, true, false);

			ImGui::SliderFloat("Collision Radius", &m_radius, 0.1f, 10);
			ImGuiFunctions::ButtonSet<float>("Reset Radius", m_radius, 1, resetSize);

			ImGui::SliderFloat3("Offset", &m_offset[0], -10, 10);
			ImGuiFunctions::ButtonSet("Reset Offset", m_offset, glm::vec3(0, 0, 0), resetSize);

			ImGuiFunctions::CloseSection(0, 2, true, true);
		}

		// Scale Settings
		{
			ImGui::Checkbox("Use Uniform Scaling", &m_useUniformScale);

			ImGuiFunctions::OpenSection(0, 0, true, false);
			if (m_useUniformScale)
			{
				ImGui::SliderFloat("Uniform Scale", &m_uniformScale, 0, 10);
				ImGuiFunctions::ButtonSet<float>("Reset UScale", m_uniformScale, 1, resetSize);

				m_scale = glm::vec3(m_uniformScale, m_uniformScale, m_uniformScale);
			}
			else
			{
				ImGui::SliderFloat3("Scale", &m_scale[0], 0, 10);
				ImGuiFunctions::ButtonSet<glm::vec3>("Reset Scale", m_scale, glm::vec3(0, 0, 0), resetSize);
			}

			ImGuiFunctions::CloseSection(0, 2, true, true);
		}

		// Rotation Settings
		ImGui::SliderFloat3("Rotation", &m_euler[0], 0, 360);
		ImGuiFunctions::ButtonSet("Reset Rotation", m_euler, glm::vec3(0, 0, 0), resetSize);

		// Position Settings
		ImGui::SliderFloat3("Position", &m_position[0], -g_gridSize, g_gridSize);
		ImGuiFunctions::ButtonSet("Reset Position", m_position, glm::vec3(0, 0, 0), resetSize);

		ImGuiFunctions::CloseSection(1, 1, true, true);

		ImGuiFunctions::ButtonSet("Delete", m_attemptDelete, true, ImVec2(50, 20));

		ImGui::End();
	}
}

bool Model::Load(std::string fileDir)
{
	// Check if the Model already has a mesh and delete it if it does
		// - Could simply exit out of the function, but this will mean you can use one Model instance to load other .obj files
	if (m_mesh != nullptr)
	{
		delete m_mesh;
	}

	m_mesh = new aie::OBJMesh();

	// Check if the Model loaded properly
	if (m_mesh->load(fileDir.c_str(), true, true) == false)
	{
		printf("Error trying to load model at: %fd\n", fileDir);
		return false;
	}
	return true;
}
