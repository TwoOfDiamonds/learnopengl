#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 color;
uniform float pushRadius;
uniform vec3 mouseLocation;

void main()
{
	color = aColor;

	 float dx = aPos.x - mouseLocation.x;
	 float dy = aPos.y - mouseLocation.y;

	float distance = sqrt(dx * dx + dy * dy);

	float pushForce = max(0, pushRadius - distance);

	gl_Position = vec4(aPos + normalize(aPos - mouseLocation) * pushForce, 1.0);
	//gl_Position = vec4(aPos, 1.0);
}