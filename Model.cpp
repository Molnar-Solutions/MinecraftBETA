#include "Model.h"
#include <iostream>
#include <algorithm>

Model::Model() : position(0.0f), scale(1.0f), rotation(0.0f)
{
}

Model::Model(const Model& other)
{
	/* Deep copy of meshList */
	for (auto mesh : other.meshList)
	{
		if (mesh)
		{
			meshList.push_back(new Mesh(*mesh));
		}
	}

	/* Deep copy of textureList */
	for (auto texture : other.textureList)
	{
		if (texture)
		{
			textureList.push_back(new Texture(*texture));
		}
	}

	/* Copy meshToTex (no deep copy needed for unsigned int vector) */
	meshToTex = other.meshToTex;
}
Model& Model::operator=(const Model& other)
{
	if (this != &other)
	{
		/* Clean up existing resources */
		for (auto mesh : meshList)
		{
			delete mesh;
		}
		meshList.clear();

		for (auto texture : textureList)
		{
			delete texture;
		}
		textureList.clear();

		/* Deep copy from other */
		for (auto mesh : other.meshList)
		{
			meshList.push_back(new Mesh(*mesh));
		}

		for (auto texture : other.textureList)
		{
			if (texture)
			{
				textureList.push_back(new Texture(*texture));
			}
		}

		meshToTex = other.meshToTex;
	}

	return *this;
}

void Model::RenderModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i];

		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			textureList[materialIndex]->UseTexture();
		}

		meshList[i]->RenderMesh();
	}
}

void Model::LoadModel(const std::string& fileName, const char* textureFolderName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		printf("Model (%s) failed to load: %s", fileName, importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene);

	LoadMaterials(scene, textureFolderName);
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene, const char* textureFolderName)
{
	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);
				std::string textureFolder = std::string(textureFolderName);

				std::string texPath = std::string("Textures/") + textureFolder + "/" + filename;

				textureList[i] = new Texture(texPath.c_str());

				if (!textureList[i]->LoadTexture())
				{
					printf("Failed to load texture at: %s\n", texPath);
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}

		if (!textureList[i])
		{
			// Default fallback texture
			textureList[i] = new Texture("Textures/plain.png");
			textureList[i]->LoadTextureA();
		}
	}
}

void Model::ClearModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		if (meshList[i])
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i])
		{
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}

/* Model specific stuffs */
void Model::setScale(float x, float y, float z)
{
	scale = glm::vec3(x, y, z);
}

void Model::setPosition(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
}

void Model::setRotation(float x, float y, float z)
{
	rotation = glm::vec3(x, y, z);
}

void Model::updateScale(float x, float y, float z)
{
	scale.x += x;
	scale.y += y;
	scale.z += z;
}

void Model::updatePosition(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
}

void Model::updateRotation(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
}

Model::~Model()
{
	/* Clean up meshList */
	for (auto mesh : meshList)
	{
		delete mesh;
	}
	meshList.clear();

	/* Clean up textureList */
	for (auto texture : textureList)
	{
		delete texture;
	}
	textureList.clear();

	meshToTex.clear();
}
