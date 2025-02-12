#pragma once

#include <vector>
#include <GL/glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Mesh
{
public:
	Mesh();
	Mesh(const Mesh& other);

	void CreateMesh(
		GLfloat* vertices,
		unsigned int* indices,
		unsigned int numOfVertices,
		unsigned int numOfIndices
	);
	void RenderMesh();
	void ClearMesh();

	std::vector<glm::vec3> GetVertices()
	{
		return vertexList;
	}

	~Mesh();
private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;

	std::vector<glm::vec3> vertexList;
};

