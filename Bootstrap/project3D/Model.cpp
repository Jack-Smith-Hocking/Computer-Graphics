#include "Model.h"
#include "Scene.h"

Model::Model(glm::vec3 pos, glm::vec3 euler, glm::vec3 scale, aie::ShaderProgram* shader) : Interactive(pos, euler, scale), m_shader(shader)
{

}

void Model::Update(float deltaTime)
{
	Object::Update(deltaTime);


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
