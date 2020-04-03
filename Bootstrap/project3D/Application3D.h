#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "Shader.h"
#include "OBJMesh.h"
#include "Texture.h"
#include "Model.h"

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	glm::mat4				m_viewMatrix;
	glm::mat4				m_projectionMatrix;

	aie::ShaderProgram*		m_phongTextured = nullptr;

	aie::OBJMesh			m_spearMesh;
	glm::mat4				m_spearTransform;

	Light*					m_light = nullptr;
	Light*					m_light2 = nullptr;
	glm::vec3				m_ambientLight = glm::vec3(1, 1, 1);

	Scene*					m_scene = nullptr;

	Model*					m_spear = nullptr;
};