#version 330 core
uniform vec2 mousePixelPosition;
uniform float radius;
in vec3 spaghetti;
out vec4 FragColor;

void main()
{
	
	float mx = gl_FragCoord.x - mousePixelPosition.x;
	float my = (800 - gl_FragCoord.y) - mousePixelPosition.y;
	float mouseDistance = sqrt(mx * mx + my * my);// * 0.3;

	if (mouseDistance > radius)
	{
		FragColor = vec4(0, 0, 0, 1);
	}
	else
	{
		FragColor = vec4(1 - clamp(mouseDistance / radius, 0.0, 1.0), 0.0, 0.0, 0.0);
	}
}