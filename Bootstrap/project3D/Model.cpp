#include "Model.h"
#include "Scene.h"
#include "imgui.h"
#include "Gizmos.h"

extern int g_gridSize;

Model::Model(glm::vec3 pos, glm::vec3 euler, glm::vec3 scale, aie::ShaderProgram* shader) : Interactive(pos, euler, scale), m_shader(shader)
{

}

Model::~Model()
{
	if (m_mesh != nullptr)
	{
		delete m_mesh;
	}
}

void Model::Update(float deltaTime)
{
	Object::Update(deltaTime);

	if (m_showMenu)
	{
		ImVec2 resetSize = ImVec2(50, 30);

		ImGui::Begin("Model Settings");

		// Collider
		{
			ImGui::Text("Collider");
			ImGui::Indent();

			ImGui::SliderFloat("Collision Radius", &m_radius, 0.1f, 10);
			ImGui::SameLine();
			if (ImGui::Button("Reset", resetSize))
			{
				m_radius = 2;
			}
			ImGui::NewLine();

			ImGui::SliderFloat3("Offset", &m_offset[0], -10, 10);
			ImGui::SameLine();
			if (ImGui::Button("Reset", resetSize))
			{
				m_offset = glm::vec3(0, 0, 0);
			}
			ImGui::NewLine();

			ImGui::Unindent();

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Separator();

			ImGui::Spacing();
			ImGui::Spacing();
		}

		// Scale
		{
			ImGui::Checkbox("Use Uniform Scaling", &m_useUniformScale);
			ImGui::Indent();

			if (m_useUniformScale)
			{
				ImGui::SliderFloat("Uniform Scale", &m_uniformScale, 0, 10);

				ImGui::SameLine();
				if (ImGui::Button("Reset", resetSize))
				{
					m_uniformScale = 1;
				}
				ImGui::NewLine();

				m_scale = glm::vec3(m_uniformScale, m_uniformScale, m_uniformScale);
			}
			else
			{
				ImGui::SliderFloat3("Scale", &m_scale[0], 0, 10);
				ImGui::SameLine();
				if (ImGui::Button("Reset", resetSize))
				{
					m_scale = glm::vec3(1, 1, 1);
				}
				ImGui::NewLine();
			}

			ImGui::Unindent();

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Separator();

			ImGui::Spacing();
			ImGui::Spacing();
		}

		ImGui::SliderFloat3("Rotation", &m_euler[0], 0, 360);
		ImGui::SameLine();
		if (ImGui::Button("Reset", resetSize))
		{
			m_euler = glm::vec3(0, 0, 0);
		}
		ImGui::NewLine();

		ImGui::SliderFloat3("Position", &m_position[0], -g_gridSize, g_gridSize);
		ImGui::SameLine();
		if (ImGui::Button("Reset", resetSize))
		{
			m_position = glm::vec3(0, 0, 0);
		}
		ImGui::NewLine();

		ImGui::End();
	}

	if (m_showCollider)
	{
		aie::Gizmos::addSphere(m_position + m_offset, m_radius, 16, 8, glm::vec4(1, 1, 1, 0));
	}
}

void Model::Draw(Scene* scene)
{
	if (scene != nullptr)
	{
		scene->UseShader(m_shader);
	}

	auto pvm = scene->GetProjectionMatrix() * scene->GetViewMatrix() * m_transform;

	m_shader->bindUniform("ProjectionViewModel", pvm);
	m_shader->bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_transform)));

	if (m_mesh != nullptr)
	{
		m_mesh->draw();
	}
}

bool Model::Load(std::string fileDir)
{
	if (m_mesh != nullptr)
	{
		delete m_mesh;
	}

	m_mesh = new aie::OBJMesh();

	if (m_mesh->load(fileDir.c_str(), true, true) == false)
	{
		printf("Error trying to load model at: %fd\n", fileDir);
		return false;
	}
	return true;
}
