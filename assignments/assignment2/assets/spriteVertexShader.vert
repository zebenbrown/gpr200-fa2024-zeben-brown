#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTextureCoordinates;

out vec4 Color;
out vec2 textureCoordinates;

uniform float scale;
void main()
{

	gl_Position = vec4(aPos.x * scale, aPos.y * scale, aPos.z, 1.0);
	Color = aColor;
	textureCoordinates = vec2(aTextureCoordinates.x, aTextureCoordinates.y);
}
