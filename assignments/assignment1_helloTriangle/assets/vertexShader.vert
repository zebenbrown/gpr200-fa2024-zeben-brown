#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTextureCoordinates;

out vec4 Color;
out vec2 textureCoordinates;

uniform float time;
uniform vec3 offsetPosition;

void main()
{
	vec3 newPos = offsetPosition * sin(time);

	gl_Position = vec4(aPos.x + newPos.x, aPos.y + newPos.y, aPos.z + newPos.z, 1.0);
	Color = aColor;
	textureCoordinates = aTextureCoordinates;
}
