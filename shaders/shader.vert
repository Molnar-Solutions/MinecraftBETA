#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;

uniform mat4 transformationMatrix;

out vec2 passTexCoords;

void main()
{
	vec4 worldPosition = transformationMatrix * vec4(pos, 1.0);

	gl_Position = worldPosition;
	passTexCoords = texCoords;
}