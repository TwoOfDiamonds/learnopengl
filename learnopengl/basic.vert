#version 330 core
uniform vec2 mousePosition;
layout (location = 0) in vec3 aPos;
out vec3 paralaxPos;

void main()
{
	float scale = 0.2;
	paralaxPos = vec3(aPos.x - mousePosition.x * scale, aPos.y - mousePosition.y * scale, aPos.z);    
	gl_Position = vec4(aPos, 1.0);
}