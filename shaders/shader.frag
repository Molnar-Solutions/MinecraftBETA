#version 330

in vec2 passTexCoords;

out vec4 color;

uniform sampler2D textureSampler;
	
void main()
{
	color = texture(textureSampler, passTexCoords);
}