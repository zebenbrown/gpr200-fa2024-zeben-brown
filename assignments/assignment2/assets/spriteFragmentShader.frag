#version 330 core


in vec4 Color;
in vec2 textureCoordinates;

out vec4 FragColor;

uniform sampler2D spriteShader;


void main()
{
	vec4 textureColor = texture(spriteShader, textureCoordinates);
	if (textureColor.a < 0.1)
	{
		discard;
	}
	FragColor = textureColor;
}