#version 330 core
out vec4 FragColor;
in vec3 paralaxPos;
uniform sampler2D myTexture;
uniform sampler2D myOtherTexture;

void main()
{
	float mx = clamp(gl_FragCoord.x / 1240, 0, 1.0);
	float my = clamp(gl_FragCoord.y / 800, 0, 1.0);
	vec2 texCoord = vec2(mx, my);
	float othermx = (paralaxPos.x + 1.0) / 2;
	float othermy = (paralaxPos.y + 1.0) / 2;
	vec2 paralaxcoord = vec2(othermx, othermy);
	vec4 texSample = texture(myTexture, texCoord);
	vec4 otherTexSample = texture(myOtherTexture, paralaxcoord);

	FragColor = texSample * (1 - otherTexSample.a) + otherTexSample * otherTexSample.a;
}