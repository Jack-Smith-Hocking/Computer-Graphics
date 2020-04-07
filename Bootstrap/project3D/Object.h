#pragma once
#include "glm/ext.hpp"

class Scene;

class Object
{
public:
	Object(glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 euler = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1));
	virtual ~Object();

	virtual void Update(float deltaTime);
	virtual void Draw(Scene* scene);

	virtual void DeleteObject();

	void UpdateTransform();

	bool ReadyForDeletion() { return m_attemptDelete; };

	glm::vec3 m_position;
	glm::vec3 m_euler;
	glm::vec3 m_scale;

protected:
	bool m_attemptDelete = false;
	bool m_hide = false;

	glm::mat4 m_transform;
};


