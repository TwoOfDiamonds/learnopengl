#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 color;

uniform mat4 transMat;

void main()
{
	color = aColor;
	gl_Position = transMat * vec4(aPos, 1.0);
}