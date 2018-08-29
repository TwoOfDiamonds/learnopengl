#version 330 core
uniform vec2 mousePosition;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 bPos;
layout (location = 2) in vec3 color;
out vec3 spaghetti;

void main()
{
	float scale = 0.2;
	vec3 finalPosition = vec3(aPos.x - mousePosition.x * scale, aPos.y - mousePosition.y * scale, aPos.z);
	
	//float mx = finalPosition.x - mousePosition.x;
	//float my = finalPosition.y - mousePosition.y;
	//mouseDistance = sqrt(mx * mx + my * my) * 0.3;
    
	gl_Position = vec4(finalPosition, 1.0);
	
	spaghetti = color;
}