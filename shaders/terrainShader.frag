#version 330

in vec4 vCol;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

in float VertexIsTopFace;

// texture uniforms
uniform sampler2D sideTexture;
uniform sampler2D topTexture;

out vec4 color;
	
void main()
{
    if (VertexIsTopFace > 0.5)  // Top-facing surfaces
      color = texture(topTexture, TexCoord);
    else
      color = texture(sideTexture, TexCoord);
}