#include "Chunk.h"

Chunk::Chunk(const Chunk& other)
{
	BLOCK_SIZE = other.BLOCK_SIZE;
	startX = other.startX;
	startZ = other.startZ;
	blockReflectivity = other.blockReflectivity;
	blockShineDamper = other.blockShineDamper;
	instanceVBO = other.instanceVBO;
	grassTexture = new Texture(*other.grassTexture);
	dirtTexture = new Texture(*other.dirtTexture);
	block = new Block(*other.block);

	for (auto& pos : other.blockPositions)
	{
		blockPositions.push_back(pos);
	}

	for (size_t i = 0; i < CHUNK_SIZE_X; i++)
	{
		for (size_t j = 0; j < CHUNK_SIZE_Y; j++)
		{
			for (size_t k = 0; k < CHUNK_SIZE_Z; k++)
			{
				blocks[i][j][k] = other.blocks[i][j][k];
			}
		}
	}
}

Chunk& Chunk::operator=(const Chunk& other)
{
	if (this != &other)
	{
		BLOCK_SIZE = other.BLOCK_SIZE;
		startX = other.startX;
		startZ = other.startZ;
		blockReflectivity = other.blockReflectivity;
		blockShineDamper = other.blockShineDamper;
		instanceVBO = other.instanceVBO;
		grassTexture = new Texture(*other.grassTexture);
		dirtTexture = new Texture(*other.dirtTexture);
		block = new Block(*other.block);

		for (auto& pos : other.blockPositions)
		{
			blockPositions.push_back(pos);
		}

		for (size_t i = 0; i < CHUNK_SIZE_X; i++)
		{
			for (size_t j = 0; j < CHUNK_SIZE_Y; j++)
			{
				for (size_t k = 0; k < CHUNK_SIZE_Z; k++)
				{
					blocks[i][j][k] = other.blocks[i][j][k];
				}
			}
		}
	}

	return *this;
}

Chunk::Chunk(float sX, float sZ)
{
	block = new Block();
	block->CreateBlock();
	block->CalculateLocalBoundingBox();
	BLOCK_SIZE = block->GetSize().x;

	LoadTextures();

	/* Build the world */
	glm::vec3 frontOffset(0, 0, -BLOCK_SIZE);
	glm::vec3 rightOffset(BLOCK_SIZE, 0, 0);
	glm::vec3 backOffset(0, 0, BLOCK_SIZE);
	glm::vec3 leftOffset(-BLOCK_SIZE, 0, 0);
	glm::vec3 topOffset(0, BLOCK_SIZE, 0);
	glm::vec3 bottomOffset(0, -BLOCK_SIZE, 0);

	for (size_t x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
		{
			for (size_t y = 0; y < CHUNK_SIZE_Y; y++)
			{
				blocks[x][y][z] = true;
			}
		}
	}

	for (size_t x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
		{
			for (size_t y = 0; y < CHUNK_SIZE_Y; y++)
			{
				glm::vec3 actualBlockPos = glm::vec3(startX, 0.0f, startZ) + glm::vec3(
					x * BLOCK_SIZE,
					y * BLOCK_SIZE,
					z * BLOCK_SIZE
				);

				if (
					!IsBlockAt(x, y, z, frontOffset) ||
					!IsBlockAt(x, y, z, backOffset) ||
					!IsBlockAt(x, y, z, rightOffset) ||
					!IsBlockAt(x, y, z, leftOffset) ||
					!IsBlockAt(x, y, z, topOffset) ||
					!IsBlockAt(x, y, z, bottomOffset)
					) {
					blockPositions.push_back(actualBlockPos);
				}
			}
		}
	}

	/* Upload the worldPositions to the gpu */
	glBindVertexArray(block->GetVao());

	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		blockPositions.size() * sizeof(glm::vec3),
		&blockPositions.front(),
		GL_STATIC_DRAW
	);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);
}

void Chunk::Render()
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, dirtTexture->GetId());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, grassTexture->GetId());

	glBindVertexArray(block->GetVao());
	glDrawElementsInstanced(GL_TRIANGLES, block->indices.size(), GL_UNSIGNED_INT, 0, blockPositions.size());
	glBindVertexArray(0);
}

bool Chunk::IsBlockAt(int x, int y, int z, glm::vec3 offset) {
	int nx = x + (int)(offset.x / BLOCK_SIZE);
	int ny = y + (int)(offset.y / BLOCK_SIZE);
	int nz = z + (int)(offset.z / BLOCK_SIZE);

	if (nx >= 0 && nx < CHUNK_SIZE_X && ny >= 0 && ny < CHUNK_SIZE_Y && nz >= 0 && nz < CHUNK_SIZE_Z) {
		return blocks[nx][ny][nz];
	}
	return false;
}

Chunk::~Chunk()
{
}

void Chunk::LoadTextures()
{
	grassTexture = new Texture("Textures/grass.png");
	grassTexture->LoadTexture();

	dirtTexture = new Texture("Textures/dirt.png");
	dirtTexture->LoadTextureA();
}
