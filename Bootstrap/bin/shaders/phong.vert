// classic Phong vertex shader
#version 410

layout( location = 0 ) in vec4 Position;
layout( location = 1 ) in vec4 Normal;
layout( location = 2 ) in vec2 TexCoord;

out vec4 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;

uniform mat4 ProjectionViewModel;

// We need this matrix to transform the position
uniform mat4 ModelMatrix;

// We need this matrix to transform the normal
uniform mat3 NormalMatrix;

void main() 
{
	vPosition = ModelMatrix * Position;
	vNormal = NormalMatrix * Normal.xyz;
	vTexCoord = vTexCoord = TexCoord;

	gl_Position = ProjectionViewModel * Position;
}