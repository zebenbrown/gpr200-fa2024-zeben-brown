#version 330 core
out vec4 FragColor;

in vec4 Color;
in vec2 textureCoordinates;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = mix(texture(texture1, textureCoordinates), texture(texture2, textureCoordinates), 0.2);
}