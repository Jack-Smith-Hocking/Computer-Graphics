#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"
#include "Texture.h"

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void drawGrid(bool doDraw);

protected:

	glm::mat4				m_viewMatrix;
	glm::mat4				m_projectionMatrix;

	aie::ShaderProgram		m_shader;
	aie::ShaderProgram		m_texturedShader;

	aie::Texture			m_gridTexture;
	Mesh					m_quadMesh;
	glm::mat4				m_quadTransform;

	aie::OBJMesh			m_bunnyMesh;
	glm::mat4				m_bunnyTransform;

	aie::OBJMesh			m_spearMesh;
	glm::mat4				m_spearTransform;
};