#version 330 core


in vec4 Color;
in vec2 textureCoordinates;

out vec4 FragColor;

uniform sampler2D spriteShader;


void main()
{
	FragColor = texture(spriteShader, textureCoordinates);
}