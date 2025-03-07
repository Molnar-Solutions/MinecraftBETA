#pragma once
#include "Shader.h"
class EntityShader : public Shader
{
public:
	EntityShader() : location_projectionMatrix(0), location_transformationMatrix(0), location_viewMatrix(0)
	{}

	void CompileProgram() override {
		Shader::CompileProgram();
		
		if (glGetError() == GL_NO_ERROR)
		{
			LoadUniformVariables();
		}
		else
		{
			std::cout << "OpenGL error found when I tried to load entityshader uniform variables!" << std::endl;
		}
	}

	void LoadUniformVariables() override
	{
		location_transformationMatrix = glGetUniformLocation(shaderID, "transformationMatrix");
		location_projectionMatrix = glGetUniformLocation(shaderID, "projection");
		location_viewMatrix = glGetUniformLocation(shaderID, "view");

		if (location_transformationMatrix == -1) std::cerr << "EntityShader - uniformModel not found :( " << std::endl;
		if (location_projectionMatrix == -1) std::cerr << "EntityShader - projectionMatrix not found :( " << std::endl;
		if (location_viewMatrix == -1) std::cerr << "EntityShader - viewMatrix not found :( " << std::endl;
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

	/* Getters for uniform variable locations */
	GLuint GetTransformationMatrixLocation() { return location_transformationMatrix; }
	GLuint GetProjectionMatrixLocation() { return location_projectionMatrix; }
	GLuint GetViewMatrixLocation() { return location_viewMatrix; }

	~EntityShader()
	{
		ClearShader();
	}
private:
	/* Uniform variable locations */
	GLuint location_transformationMatrix,
		location_projectionMatrix,
		location_viewMatrix;
};

