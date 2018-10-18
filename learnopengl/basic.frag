#version 330 core

out vec4 FragColor;

in vec3 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPosition;

uniform sampler2D texture0;

void main()
{
	//vec4 textureColor = texture(texture0, vec2(TexCoords.x, 1 - TexCoords.y)).bgra;
	vec4 textureColor = vec4(1.0, 1.0, 1.0, 1.0);

	if (textureColor.a < 0.7)
		discard;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPosition - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);

	FragColor = vec4(diffuse * vec3(textureColor.r, textureColor.g, textureColor.b), textureColor.a);
	//FragColor = vec4(norm, 1.0);
	//FragColor = textureColor;
}