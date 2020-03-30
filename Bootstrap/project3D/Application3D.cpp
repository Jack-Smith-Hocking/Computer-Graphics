#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {

	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.f);

	// Load shader files
	m_shader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
	m_shader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	// Check if there was any errors linking the shader files
	if (m_shader.link() == false)
	{
		printf("Shader Error: %s\n", m_shader.getLastError());
		return false;
	}

	// Load texture shader files
	m_texturedShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");

	// Check if there was any errors linking the shader files
	if (m_texturedShader.link() == false)
	{
		printf("TexturedShader Error: %s\n", m_texturedShader.getLastError());
		return false;
	}

	// Load phong shader files
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");

	// Check if there was any errors linking the shader files
	if (m_phongShader.link() == false)
	{
		printf("TexturedShader Error: %s\n", m_phongShader.getLastError());
		return false;
	}

	if (m_bunnyMesh.load("./models/Dragon.obj") == false)
	{
		printf("Bunny Mesh Error!\n");
		return false;
	}

	m_bunnyTransform =
	{
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1
	};

	// Check if texture loads
	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("Failed to load texture!\n");
		return false;
	}

	// Initialise the quad's mesh
	m_quadMesh.InitialiseQuad();

	// Make the quad 10 units wide
	m_quadTransform = 
	{
		10, 0, 0, 0,
		0, 10, 0, 0,
		0, 0, 10, 0,
		0, 0, 0, 1
	};

	if (m_spearMesh.load("./models/soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soulspear Mesh Error!\n");
		return false;
	}

	m_spearTransform =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	m_light.diffuse = { 1, 1, 0 };
	m_light.specular = { 1, 1, 0 };
	m_ambientLight = { 0.25f, 0.25f, 0.25f };

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
}

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	// rotate camera
	//m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 25, 10, glm::cos(time) * 25), vec3(0), vec3(0, 1, 0));

	// rotate light
	m_light.direction = glm::normalize(vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	drawGrid(true);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.f);

	// Bind shader
	m_shader.bind();

	// Bind textured Shader
	m_texturedShader.bind();

	// Bind phong shader
	m_phongShader.bind();

	// Bind transform
	auto pvm = m_projectionMatrix * m_viewMatrix * m_spearTransform;
	m_shader.bindUniform("ProjectionViewModel", pvm);

	// Bind textured transform
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);
	// Bind texture location
	m_texturedShader.bindUniform("diffuseTexture", 0);

	// Bind the phong transform
	m_phongShader.bindUniform("ProjectionViewModel", pvm);
	// Bind the transform to rotate the normal by
	m_phongShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_spearTransform)));
	// Bind light
	m_phongShader.bindUniform("Ia", m_ambientLight);
	m_phongShader.bindUniform("Id", m_light.diffuse);
	m_phongShader.bindUniform("Is", m_light.specular);
	m_phongShader.bindUniform("LightDirection", m_light.direction);
	// Bind camera position
	m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(m_viewMatrix)[3]));

	// Bind texture to specified location
	m_gridTexture.bind(0);

	// Draw quad
	m_quadMesh.Draw();
	//m_bunnyMesh.draw();

	m_spearMesh.draw();

	// draw 3D gizmos
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}

void Application3D::drawGrid(bool doDraw)
{
	if (!doDraw) return;

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) 
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);

		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}
}
