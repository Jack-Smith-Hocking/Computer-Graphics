#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Scene.h"
#include "imgui.h"
#include <string>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

glm::vec4 g_selectedColour = glm::vec4(1, 0, 0, 0.2f);

unsigned int g_windowHeight;
unsigned int g_windowWidth;

int g_gridSize;

std::string g_soulSpearDir = "./models/soulspear/soulspear.obj";
std::string g_bunnyDir = "./models/Bunny.obj";
std::string g_dragonDir = "./models/Dragon.obj";
std::string g_buddhaDir = "./models/Buddha.obj";
std::string g_lucyDir = "./models/Lucy.obj";

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

	aie::ShaderProgram* untextured = new aie::ShaderProgram();
	untextured->loadShader(aie::eShaderStage::VERTEX, "./shaders/phongUntextured.vert");
	untextured->loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phongUntextured.frag");
	// Check if there was any errors linking the shader files
	if (untextured->link() == false)
	{
		printf("UntexturedShader Error: %s\n", untextured->getLastError());
		return false;
	}

	aie::ShaderProgram* textured = new aie::ShaderProgram();
	textured->loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
	textured->loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");
	// Check if there was any errors linking the shader files
	if (textured->link() == false)
	{
		printf("TexturedShader Error: %s\n", textured->getLastError());
		return false;
	}

	m_scene = new Scene(new Camera());
	m_scene->SetShaders(textured, untextured);

	Light* light = new Light(glm::vec3(0, -20, 0), glm::vec3(1, 1, 1));
	light->m_intensity = 1000;
	m_scene->AddLight(light);

	Light* light2 = new Light(glm::vec3(0, 20, 0), glm::vec3(1, 1, 1));
	light2->m_intensity = 1000;
	m_scene->AddLight(light2);

	Model* spearModel = new Model(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), textured);
	spearModel->Load(g_soulSpearDir.c_str());

	m_scene->AddModel(spearModel);

	spearModel = new Model(glm::vec3(5, 0, 5), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), textured);
	spearModel->Load(g_soulSpearDir.c_str());

	m_scene->AddModel(spearModel);

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

