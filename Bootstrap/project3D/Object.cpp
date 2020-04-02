#include "Object.h"

Object::Object(glm::vec3 pos, glm::vec3 euler, glm::vec3 scale) : m_position(pos), m_euler(euler), m_scale(scale)
{
	UpdateTransform();
}

Object::~Object()
{
}

void Object::Update(float deltaTime)
{
	UpdateTransform();
}

void Object::Draw(Scene* scene)
{
}

void Object::DeleteObject()
{
	delete this;
}

void Object::UpdateTransform()
{
	float rad = 6.28f / 360.0f;

	m_transform = glm::translate(glm::mat4(1), m_position)
		* glm::rotate(glm::mat4(1), m_euler.z * rad, glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), m_euler.y * rad, glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1), m_euler.x * rad, glm::vec3(1, 0, 0))
		* glm::scale(glm::mat4(1), m_scale);
}
