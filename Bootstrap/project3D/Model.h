#pragma once
#include "Interactive.h"

#include <string>
#include "OBJMesh.h"
#include "Shader.h"
#include "glm/ext.hpp"

class Model : public Interactive
{
public:

	Model(glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 euler = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1), aie::ShaderProgram* shader = nullptr);

	void Update(float deltaTime) override;
	void Draw(Scene* scene) override;

	bool Load(std::string fileDir);

protected:

	aie::OBJMesh* m_mesh = nullptr;
	aie::ShaderProgram* m_shader = nullptr;
};

