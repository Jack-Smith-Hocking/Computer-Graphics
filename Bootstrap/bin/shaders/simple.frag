// a simple flat colour shader

#version 410

uniform vec3 Kd;

out vec4 FragColour;

void main()
{
	FragColour = vec4(1, 0, 1, 1);
	//FragColour = vec4(Kd, 1);
}