#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Scene.h"
#include "imgui.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

unsigned int g_windowHeight;
unsigned int g_windowWidth;

int g_gridSize;

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
	m_phongUntextured.loadShader(aie::eShaderStage::VERTEX, "./shaders/phongUntextured.vert");
	m_phongUntextured.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phongUntextured.frag");

	// Check if there was any errors linking the shader files
	if (m_phongUntextured.link() == false)
	{
		printf("TexturedShader Error: %s\n", m_phongUntextured.getLastError());
		return false;
	}

	// Load phong shader files
	m_phongTextured.loadShader(aie::eShaderStage::VERTEX, "./shaders/phongTextured.vert");
	m_phongTextured.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phongTextured.frag");

	// Check if there was any errors linking the shader files
	if (m_phongTextured.link() == false)
	{
		printf("TexturedShader Error: %s\n", m_phongTextured.getLastError());
		return false;
	}

	if (m_bunnyMesh.load("./models/Bunny.obj") == false)
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

	m_light = new Light(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), 2);
	m_light2 = new Light(glm::vec3(10, 0, 0), glm::vec3(1, 0, 0), 2);
	m_ambientLight = { 0.25f, 0.25f, 0.25f };

	m_scene = new Scene();
	m_scene->AddLight(m_light);
	m_scene->AddLight(m_light2);

	m_camera = new Camera(m_scene);
	
	g_windowHeight = getWindowHeight();
	g_windowWidth = getWindowWidth();

	g_gridSize = 20;

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
}

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	ImGui::Begin("Scene Settings");
	ImGui::SliderInt("Grid Size", &g_gridSize, 10, 250);
	ImGui::End();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	drawGrid(g_gridSize, true);
	m_scene->Update(deltaTime);

	m_camera->Update(deltaTime);

	g_windowHeight = getWindowHeight();
	g_windowWidth = getWindowWidth();

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = m_camera->GetProjectionMatrix(getWindowWidth(), getWindowHeight()); //glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.f);
	m_viewMatrix = m_camera->GetViewMatrix();

	// Bind shader
	m_shader.bind();

	// Bind transform
	auto pvm = m_projectionMatrix * m_viewMatrix * m_spearTransform;
	m_shader.bindUniform("ProjectionViewModel", pvm);

	{
		// Bind phong shader
		m_phongTextured.bind();

		// Bind the phong transform
		m_phongTextured.bindUniform("ProjectionViewModel", pvm);
		// Bind the transform to rotate the normal by
		m_phongTextured.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_spearTransform)));
		// Bind light
		m_phongTextured.bindUniform("Ia", m_ambientLight);

		glm::vec3 pos[2];
		glm::vec3 col[2];
		float in[2];

		pos[0] = m_light->m_position;
		pos[1] = m_light2->m_position;

		col[0] = m_light->m_colour;
		col[1] = m_light2->m_colour;

		in[0] = m_light->m_intensity;
		in[1] = m_light2->m_intensity;

		m_phongTextured.bindUniform("LightColour", 2, &col[0]);
		m_phongTextured.bindUniform("LightPosition", 2, &pos[0]);
		m_phongTextured.bindUniform("LightIntensity", 2, &in[0]);
		// Bind camera position
		m_phongTextured.bindUniform("CameraPosition", vec3(glm::inverse(m_viewMatrix)[3]));

		m_spearMesh.draw();
		//m_bunnyMesh.draw();
	}

	// Bind transform
	pvm = m_projectionMatrix * m_viewMatrix * m_bunnyTransform;
	m_shader.bindUniform("ProjectionViewModel", pvm);
	{
		// Bind textured Shader
		//m_texturedShader.bind();

		// Bind textured transform
		m_phongUntextured.bindUniform("ProjectionViewModel", pvm);
		// Bind the transform to rotate the normal by
		m_phongUntextured.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_spearTransform)));
		// Bind light
		m_phongUntextured.bindUniform("Ia", m_ambientLight);
		m_phongUntextured.bindUniform("colour", m_light->m_colour);
		m_phongUntextured.bindUniform("LightPosition", m_light->m_position);
		// Bind camera position
		m_phongUntextured.bindUniform("CameraPosition", vec3(glm::inverse(m_viewMatrix)[3]));


		// Bind texture to specified location
		//m_gridTexture.bind(0);

		// Draw quad
		//m_quadMesh.Draw();
	}

	// draw 3D gizmos
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}

void Application3D::drawGrid(int halfGridSize, bool doDraw)
{
	if (!doDraw) return;

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	vec4 blue(0, 0, 1, 1);
	vec4 red(1, 0, 0, 1);
	vec4 green(0, 1, 0, 1);

	int gridSize = 2 * halfGridSize;

	Gizmos::addLine(vec3(0, -halfGridSize, 0), vec3(0, halfGridSize, 0), green);

	for (int i = 0; i < gridSize + 1; i++) 
	{
		Gizmos::addLine(vec3(-halfGridSize + i, 0, halfGridSize), vec3(-halfGridSize + i, 0, -halfGridSize), i == halfGridSize ? blue : black);

		Gizmos::addLine(vec3(halfGridSize, 0, -halfGridSize + i), vec3(-halfGridSize, 0, -halfGridSize + i), i == halfGridSize ? red : black);
	}
}
