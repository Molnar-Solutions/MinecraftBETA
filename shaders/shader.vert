#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;

out vec2 passTexCoords;

void main()
{
	gl_Position = vec4(pos, 1.0);
	passTexCoords = texCoords;
}