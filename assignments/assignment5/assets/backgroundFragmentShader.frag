#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPosition;
in vec2 textureCoordinates;

uniform sampler2D texture1;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(lightPosition - FragPosition);

	float diff = max(dot(norm, lightDirection), 0.0f);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * vec3(texture(texture1, textureCoordinates)).rgb;
	FragColor = vec4(result, 1.0f);
}