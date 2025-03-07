#pragma once
#include "Shader.h"
class TerrainShader : public Shader
{
public: 
	TerrainShader() : location_projectionMatrix(0), location_transformationMatrix(0), location_viewMatrix(0),
		location_SideTextureLocation(0), location_TopTextureLocation(0)
	{}

	void CompileProgram() override {
		Shader::CompileProgram();

		if (glGetError() == GL_NO_ERROR)
		{
			LoadUniformVariables();
		}
		else
		{
			std::cout << "OpenGL error found when I tried to load TerrainShader uniform variables!" << std::endl;
		}
	}

	void LoadUniformVariables() override
	{
		location_transformationMatrix = glGetUniformLocation(shaderID, "transformationMatrix");
		location_projectionMatrix = glGetUniformLocation(shaderID, "projection");
		location_viewMatrix = glGetUniformLocation(shaderID, "view");
		location_SideTextureLocation = glGetUniformLocation(shaderID, "sideTexture");
		location_TopTextureLocation = glGetUniformLocation(shaderID, "topTexture");

		if (location_transformationMatrix == -1) std::cerr << "TerrainShader - uniformModel not found :( " << std::endl;
		if (location_projectionMatrix == -1) std::cerr << "TerrainShader - projectionMatrix not found :( " << std::endl;
		if (location_viewMatrix == -1) std::cerr << "TerrainShader - viewMatrix not found :( " << std::endl;
	}

	/* Setters for uniform variables */
	void SetTransformationMatrix(glm::mat4 model)
	{
		glUniformMatrix4fv(location_transformationMatrix, 1, GL_FALSE, glm::value_ptr(model));
	}

	void SetProjectionMatrix(glm::mat4 model)
	{
		glUniformMatrix4fv(location_projectionMatrix, 1, GL_FALSE, glm::value_ptr(model));
	}

	void SetViewMatrix(glm::mat4 model)
	{
		glUniformMatrix4fv(location_viewMatrix, 1, GL_FALSE, glm::value_ptr(model));
	}

	void SetTopTexture(GLuint textureUnit)
	{
		glUniform1i(location_TopTextureLocation, textureUnit);
	}

	void SetSideTexture(GLuint textureUnit)
	{
		glUniform1i(location_SideTextureLocation, textureUnit);
	}

	/* Getters for uniform variable locations */
	GLuint GetTransformationMatrixLocation() { return location_transformationMatrix; }
	GLuint GetProjectionMatrixLocation() { return location_projectionMatrix; }
	GLuint GetViewMatrixLocation() { return location_viewMatrix; }
	GLuint GetSideTextureLocation() { return location_SideTextureLocation; }
	GLuint GetTopTextureLocation() { return location_TopTextureLocation; }

	~TerrainShader()
	{
		ClearShader();
	}
private:
	/* Uniform variable locations */
	GLuint location_transformationMatrix,
		location_projectionMatrix,
		location_viewMatrix,
		location_SideTextureLocation,
		location_TopTextureLocation;
};

