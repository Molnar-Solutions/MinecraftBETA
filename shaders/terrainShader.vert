#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
layout (location = 3) in vec3 instancePosition;
layout (location = 4) in float isTopFace;

uniform mat4 transformationMatrix;
uniform mat4 projection;
uniform mat4 view;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out float VertexIsTopFace;

void main()
{
	mat4 translationMatrix = mat4(1.0);
	translationMatrix[3][0] = instancePosition.x;
    translationMatrix[3][1] = instancePosition.y;
    translationMatrix[3][2] = instancePosition.z;

	vec4 worldPosition = translationMatrix * vec4(pos, 1.0);

	gl_Position = projection * view * worldPosition;

	TexCoord = tex;
	FragPos = worldPosition.xyz;
	Normal = mat3(transpose(inverse(transformationMatrix))) * norm;
	VertexIsTopFace = isTopFace;
}