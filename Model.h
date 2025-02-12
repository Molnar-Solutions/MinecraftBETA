#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
	Model();
	Model(const Model& other);
	Model& operator=(const Model& other);

	void LoadModel(const std::string& fileName,
		const char* textureFolderName);
	void RenderModel();
	void ClearModel();

	void setScale(float x, float y, float z); 
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);

	void updateScale(float x, float y, float z);
	void updatePosition(float x, float y, float z);
	void updateRotation(float x, float y, float z);

	glm::vec3 GetScale() { return scale; }
	glm::vec3 GetRotation() { return rotation; }
	glm::vec3 GetPosition() { return position; }

	float GetWidth() { return width; }
	float GetHeight() { return height; }
	float GetDepth() { return depth; }

	glm::vec3 GetMinBounds() { return minBounds; }
	glm::vec3 GetMaxBounds() { return maxBounds; }
	glm::vec3 GetDimensions()
	{
		/* width, height, depth */
		return maxBounds - minBounds;
	}

	/* AABB (Axis-Aligned Bounding Box) */
	void calculateBoundingBox()
	{
		minBounds = glm::vec3(FLT_MAX);
		maxBounds = glm::vec3(-FLT_MAX);

		for (Mesh* mesh : meshList)
		{
			for (const glm::vec3& vertex : mesh->GetVertices())
			{
				minBounds.x = std::min(minBounds.x, vertex.x);
				minBounds.y = std::min(minBounds.y, vertex.y);
				minBounds.z = std::min(minBounds.z, vertex.z);

				maxBounds.x = std::max(maxBounds.x, vertex.x);
				maxBounds.y = std::max(maxBounds.y, vertex.y);
				maxBounds.z = std::max(maxBounds.z, vertex.z);
			}
		}

		width = maxBounds.x - minBounds.x;
		height = maxBounds.y - minBounds.y;
		depth = maxBounds.z - minBounds.z;
	}

	~Model();
private:
	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene,
		const char* textureFolderName);

	float rotationAngle = 20.0f;

	float width;
	float height;
	float depth;

	/* Minimum XYZ coordinates */
	glm::vec3 minBounds;
	/* Minimum XYZ coordinates */
	glm::vec3 maxBounds;

	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec3 position;

	std::vector<Mesh*> meshList;
	std::vector<Texture*> textureList;
	std::vector<unsigned int> meshToTex;
};

