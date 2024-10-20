#version 330 core

in vec4 Color;
in vec2 textureCoordinates;

out vec4 FragColor;

uniform sampler2D backgroundShader;
uniform sampler2D texture2;

void main()
{
	FragColor = texture(backgroundShader, textureCoordinates);
}