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

	Scene*					m_scene = nullptr;
};