#pragma once
#include "Interactable.h"

#include <string>
#include "OBJMesh.h"
#include "Shader.h"
#include "glm/ext.hpp"

class Model : public Interactable
{
public:

	Model(glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 euler = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1), aie::ShaderProgram* shader = nullptr);

	~Model() override;

	void Draw(Scene* scene) override;

	void UpdateImGui() override;

	bool Load(std::string fileDir);

protected:
	
	bool m_useUniformScale = true;				// Determines whether to use the same scalar value to scale the model by
	
	float m_uniformScale = 1;					// For scaling a model's xyz by the same scalar value

	aie::OBJMesh* m_mesh = nullptr;
	aie::ShaderProgram* m_shader = nullptr;
};

