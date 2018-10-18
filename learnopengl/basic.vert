#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 texCoord;
layout (location = 2) in vec3 normal;

out vec3 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec3 LightPosition;

uniform mat4 transMat; //this is model matrix
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPos;

void main()
{
	Normal = mat3(transpose(inverse(transMat))) * normal;
	TexCoords = texCoord;
	FragPos = vec3(transMat * vec4(aPos, 1.0));
	LightPosition = lightPos;
	gl_Position = projectionMatrix * viewMatrix * transMat * vec4(aPos, 1.0);
}