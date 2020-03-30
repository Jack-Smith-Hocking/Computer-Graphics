#include "Mesh.h"
#include <gl_core_4_4.h>

Mesh::~Mesh()
{
	// Cleanup buffers and arrays
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void Mesh::Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount, unsigned int* indices)
{
	// Check that the mesh hasn't been initialised already
	assert(m_vao == 0);

	// Generate buffers
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

	// Bind vertex array aka a mesh wrapper
	glBindVertexArray(m_vao);

	// Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// Enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Enable second element as texture coordinate
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

	// Bind indices if there are any
	if (indexCount != 0)
	{
		glGenBuffers(1, &m_ibo);

		// Bind vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_BINDING, m_ibo);

		// Fill vertex buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		m_triCount = indexCount / 3;
	}
	else
	{
		m_triCount = vertexCount / 3;
	}

	// Unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::InitialiseQuad()
{
	// Defining 6 vertices for 2 triangles
	Vertex vertices[6];
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };

	vertices[3].position = { -0.5f, 0, -0.5f, 1 };
	vertices[4].position = { 0.5f, 0, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0, -0.5f, 1 };

	// Define texture coordinates 
	vertices[0].texCoord = { 0, 1 }; // Bottom left
	vertices[1].texCoord = { 1, 1 }; // Bottom right
	vertices[2].texCoord = { 0, 0 }; // Top left

	vertices[3].texCoord = { 0, 0 }; // Top left
	vertices[4].texCoord = { 1, 1 }; // Bottom right
	vertices[5].texCoord = { 1, 0 }; // Top right

	// Initialise a quad
	Initialise(6, vertices);
}

void Mesh::Draw()
{
	// Bind vertex array
	glBindVertexArray(m_vao);

	// Check whether using indices or just vertices
	if (m_ibo != 0)
	{
		glDrawElements(GL_TRIANGLES, 3 * m_triCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 3 * m_triCount);
	}
}
