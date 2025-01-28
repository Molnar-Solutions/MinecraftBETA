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

	~Shader();
private:
	GLuint shaderID;

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

