#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Block.h"
#include "Texture.h"

#define CHUNK_SIZE_X 5
#define CHUNK_SIZE_Y 5
#define CHUNK_SIZE_Z 5

class Chunk
{
public:
	Chunk() {}
	Chunk(float sX, float sZ);
	Chunk(const Chunk& other);
	Chunk& operator=(const Chunk& other);

	void Render();
	bool IsBlockAt(int x, int y, int z, glm::vec3 offset);

	glm::vec3 GetPosition() {
		return glm::vec3(startX, 0, startZ);
	}

	glm::vec3 GetSize() {
		return glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);
	}

	GLfloat GetBlockSize() { return BLOCK_SIZE; }

	std::vector<glm::vec3> GetBlockPositions()
	{
		return blockPositions;
	}

	~Chunk();
private:
	bool blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z] = { false };

	void LoadTextures();

	Block* block;

	float startX, startZ;
	float blockReflectivity, blockShineDamper;

	GLuint instanceVBO;
	GLfloat BLOCK_SIZE;

	Texture* grassTexture;
	Texture* dirtTexture;

	std::vector<glm::vec3> blockPositions;
};

