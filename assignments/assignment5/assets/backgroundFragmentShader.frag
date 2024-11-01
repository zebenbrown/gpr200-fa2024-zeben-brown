#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPosition;
in vec2 textureCoordinates;

uniform sampler2D texture1;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;

void main()
{
	vec3 color = texture(texture1, textureCoordinates).rgb;
	//ambient
	vec3 ambient = 0.05f * color;

	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(lightPosition - FragPosition);
	float diff = max(dot(norm, lightDirection), 0.0f);
	vec3 diffuse = diff * color;

	//blinn-phong
	vec3 viewDirection = normalize(viewPosition - FragPosition);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = 0.0f;
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	spec = pow(max(dot(norm, halfwayDirection), 0.0), 32.0);
	vec3 specular = vec3(0.3f) * spec;


	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}