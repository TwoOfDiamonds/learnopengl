#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 texCoord;
layout (location = 2) in vec3 normal;

out vec3 TexCoords;

uniform mat4 transMat;

void main()
{
	TexCoords = texCoord;
	gl_Position = transMat * vec4(aPos, 1.0);
}