#pragma once
#include <glm/ext.hpp>

class Mesh
{
public:
	struct Vertex
	{
	public:
		glm::vec4 position = glm::vec4(0, 0, 0, 0);
		glm::vec4 normal = glm::vec4(0, 0, 0, 0);
		glm::vec4 texCoord = glm::vec4(0, 0, 0, 0);
	};

	Mesh() : m_triCount(0), m_vao(0), m_vbo(0), m_ibo(0) {}
	virtual ~Mesh();

	// Initialises an arbitrary mesh
	void Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);

	// Initialises a quad
	void InitialiseQuad();

	// Draws the mesh
	virtual void Draw();

private:
	unsigned int m_triCount;
	unsigned int m_vao, m_vbo, m_ibo;
};

