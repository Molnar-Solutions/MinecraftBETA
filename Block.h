#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Block
{
public:
	Block();
	Block(const Block& other);
	Block& operator=(const Block& other);

	void CreateBlock();
	void CalculateLocalBoundingBox();
	void CalculateWorldBoundingBox();

	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	glm::vec3 GetLocalMinBounds() const;
	glm::vec3 GetLocalMaxBounds() const;

	glm::vec3 GetWorldMinBounds() const;
	glm::vec3 GetWorldMaxBounds() const;

	GLuint GetVao() const { return VAO; }
	GLuint GetVBO() const { return VBO; }
	GLuint GetIBO() const { return IBO; }

	glm::vec3 GetSize() const { return size; }

	~Block()
	{
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &IBO);
		glDeleteVertexArrays(1, &VAO);
	}
private:
	GLuint VAO, VBO, IBO, vertexCount, indiceCount,
		strideLength;

	glm::vec3 size;

	void StoreInVAO();
	void InitialiseBlockData();
};

