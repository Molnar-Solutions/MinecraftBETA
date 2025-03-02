#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>

class Shader
{
public:
	Shader();

	void CreateFromString(
		const char* vertexCode,
		const char* fragmentCode
	);
	void CreateFromFiles(
		const char* vertexLocation,
		const char* fragmentLocation
	);

	void Validate();

	std::string ReadFile(
		const char* fileLocation
	);

	void UseShader();
	void ClearShader();
	void StopShader() { glUseProgram(0); }

	void CompileProgram();

	void LoadUniformVariables();

	/* Setters for uniform variables */
	void SetTransformationMatrix(glm::mat4 model);
	void SetProjectionMatrix(glm::mat4 model);
	void SetViewMatrix(glm::mat4 model);

	/* Getters for uniform variable locations */
	GLuint GetTransformationMatrixLocation() { return location_transformationMatrix; }
	GLuint GetProjectionMatrixLocation() { return location_projectionMatrix; }
	GLuint GetViewMatrixLocation() { return location_viewMatrix; }

	~Shader();
private: 
	GLuint shaderID;

	/* Uniform variable locations */
	GLuint location_transformationMatrix,
		location_projectionMatrix,
		location_viewMatrix;

	void CompileShader(
		const char* vertexCode,
		const char* fragmentCode
	);
	void AddShader(
		GLuint theProgram,
		const char* shaderCode,
		GLenum shaderType
	);
};

