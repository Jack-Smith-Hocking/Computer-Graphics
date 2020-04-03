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

glm::vec4 g_selectedColour = glm::vec4(1, 0, 0, 0.2f);

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

	m_phongTextured = new aie::ShaderProgram();

	// Load phong shader files
	m_phongTextured->loadShader(aie::eShaderStage::VERTEX, "./shaders/phongTextured.vert");
	m_phongTextured->loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phongTextured.frag");

	// Check if there was any errors linking the shader files
	if (m_phongTextured->link() == false)
	{
		printf("TexturedShader Error: %s\n", m_phongTextured->getLastError());
		return false;
	}

	//if (m_spearMesh.load("./models/soulspear/soulspear.obj", true, true) == false)
	//{
	//	printf("Soulspear Mesh Error!\n");
	//	return false;
	//}

	//m_spearTransform =
	//{
	//	1, 0, 0, 0,
	//	0, 1, 0, 0,
	//	0, 0, 1, 0,
	//	0, 0, 0, 1
	//};

	m_light = new Light(glm::vec3(-10, 0, 0), glm::vec3(0, 1, 0));
	m_light->m_intensity = 20;

	m_light2 = new Light(glm::vec3(10, 0, 0), glm::vec3(1, 0, 0));
	m_light2->m_intensity = 20;

	m_ambientLight = { 0.25f, 0.25f, 0.25f };

	m_scene = new Scene(new Camera());
	m_scene->AddLight(m_light);
	m_scene->AddLight(m_light2);

	m_spear = new Model(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), m_phongTextured);
	m_spear->Load("./models/soulspear/soulspear.obj");

	m_scene->AddModel(m_spear);

	m_spear = new Model(glm::vec3(5, 0, 5), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), m_phongTextured);
	m_spear->Load("./models/soulspear/soulspear.obj");

	m_scene->AddModel(m_spear);

	g_windowHeight = getWindowHeight();
	g_windowWidth = getWindowWidth();

	g_gridSize = 20;

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
}

void Application3D::update(float deltaTime) {

	if (ImGui::IsWindowCollapsed())
	{
		return;
	}

	// query time since application started
	float time = getTime();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	m_scene->Update(deltaTime);

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

	if (ImGui::IsWindowCollapsed())
	{
		return;
	}

	// update perspective in case window resized
	m_projectionMatrix = m_scene->GetProjectionMatrix();
	m_viewMatrix = m_scene->GetViewMatrix();

	m_scene->Draw();

	// draw 3D gizmos
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}

