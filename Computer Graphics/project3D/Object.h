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

	glm::vec3 m_position;				// Position of the Object in world space
	glm::vec3 m_euler;					// Rotation of the Object in world space
	glm::vec3 m_scale;					// Scale of the Object in world space

protected:
	bool m_attemptDelete = false;		// Whether the Object is marked for deletion or not
	bool m_hide = false;				// Whether the Object should draw itself or not

	glm::mat4 m_transform;				// Transform that wraps the position, rotation and scale into a mat4
};


