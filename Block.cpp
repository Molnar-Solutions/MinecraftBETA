#include "Block.h"

Block::Block()
{
	
}

Block::Block(const Block& other)
{
	VAO = other.VAO;
	VBO = other.VBO;
	IBO = other.IBO;
	vertexCount = other.vertexCount;
	indiceCount = other.indiceCount;
	strideLength = other.strideLength;
	vertices = other.vertices;
	indices = other.indices;
}

Block& Block::operator=(const Block& other)
{
	if (this != &other)
	{
		VAO = other.VAO;
		VBO = other.VBO;
		IBO = other.IBO;
		vertexCount = other.vertexCount;
		indiceCount = other.indiceCount;
		strideLength = other.strideLength;
		vertices = other.vertices;
		indices = other.indices;
	}

	return *this;
}

void Block::CreateBlock()
{
	InitialiseBlockData();
	StoreInVAO();
	CalculateLocalBoundingBox();
	CalculateWorldBoundingBox();
}

void Block::StoreInVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(),
		GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices.front(),
		GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strideLength * sizeof(vertices[0]), (void*)0);
	glEnableVertexAttribArray(0);
	 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, strideLength * sizeof(vertices[0]), (void*) (sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, strideLength * sizeof(vertices[0]), (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, strideLength * sizeof(vertices[0]), (void*)(sizeof(vertices[0]) * 8));
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Block::InitialiseBlockData()
{
	vertices = {
		// Position         // Texture   // Normal             // Is top face

		// Front face
	   -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,   0.0f,  0.0f,  1.0f,	0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 1.0f,   0.0f,  0.0f,  1.0f,	0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  0.0f,  1.0f,	0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  0.0f,  1.0f,	0.0f,

	   // Right face
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,   1.0f,  0.0f,  0.0f,	0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,   1.0f,  0.0f,  0.0f,	0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.0f,   1.0f,  0.0f,  0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 0.0f,   1.0f,  0.0f,  0.0f,	0.0f,

		// Back face
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  0.0f, -1.0f,	0.0f,
	   -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,   0.0f,  0.0f, -1.0f,	0.0f,
	   -0.5f,  0.5f, -0.5f,  1.0f, 0.0f,   0.0f,  0.0f, -1.0f,	0.0f,
		0.5f,  0.5f, -0.5f,  0.0f, 0.0f,   0.0f,  0.0f, -1.0f,	0.0f,

		// Left face
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,	0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,	0.0f,

		// Top face (seperate texture)
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   0.0f,  1.0f,  0.0f,	1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0.0f,  1.0f,  0.0f,	1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,   0.0f,  1.0f,  0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,   0.0f,  1.0f,  0.0f,	1.0f,

		// Bottom face
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0.0f, -1.0f,  0.0f,	0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,   0.0f, -1.0f,  0.0f,	0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f, -1.0f,  0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f, -1.0f,  0.0f,	0.0f // vertex 
	};

	indices = {
		0, 1, 2,  2, 3, 0,  // Front
		4, 5, 6,  6, 7, 4,  // Right
		8, 9, 10, 10, 11, 8, // Back
		12, 13, 14, 14, 15, 12, // Left
		16, 17, 18, 18, 19, 16, // Top
		20, 21, 22, 22, 23, 20  // Bottom
	};

	strideLength = 9;
	indiceCount = indices.size();
	vertexCount = vertices.size() / strideLength;
}

void Block::CalculateLocalBoundingBox()
{
	glm::vec3 minBounds(FLT_MAX, FLT_MAX, FLT_MAX);
	glm::vec3 maxBounds(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (size_t i = 0; i < vertices.size(); i += strideLength)
	{
		float x = vertices[i];
		float y = vertices[i + 1];
		float z = vertices[i + 2];

		minBounds.x = std::min(minBounds.x, x);
		minBounds.y = std::min(minBounds.y, y);
		minBounds.z = std::min(minBounds.z, z);

		maxBounds.x = std::max(maxBounds.x, x);
		maxBounds.y = std::max(maxBounds.y, y);
		maxBounds.z = std::max(maxBounds.z, z);
	}

	double width = maxBounds.x - minBounds.x;
	double height = maxBounds.y - minBounds.y;
	double depth = maxBounds.z - minBounds.z;

	size = glm::vec3(width, height, depth);
}

void Block::CalculateWorldBoundingBox()
{
}

glm::vec3 Block::GetLocalMinBounds() const
{
	return glm::vec3();
}

glm::vec3 Block::GetLocalMaxBounds() const
{
	return glm::vec3();
}

glm::vec3 Block::GetWorldMinBounds() const
{
	return glm::vec3();
}

glm::vec3 Block::GetWorldMaxBounds() const
{
	return glm::vec3();
}
