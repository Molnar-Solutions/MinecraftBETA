#pragma once

#include <stdio.h>

#include <GL/glew.h>

#include "CommonValues.h"

class Texture
{
public:
	Texture();
	Texture(const Texture& other);
	Texture(const char* fileLocation);

	/* load texture without alpha channel */
	bool LoadTexture();
	/* load texture with alpha */
	bool LoadTextureA();

	void UseTexture();
	void ClearTexture();

	GLuint GetId() { return textureID; }

	~Texture();
private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
};

