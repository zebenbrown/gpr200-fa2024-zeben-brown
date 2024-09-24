#version 330 core
out vec4 FragColor;

in vec4 Color;
in vec2 textureCoordinates;

uniform sampler2D myTexture;
void main()
{
	FragColor = texture(myTexture, textureCoordinates);
}