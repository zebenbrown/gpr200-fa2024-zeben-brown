#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 2) in vec4 aColor;
layout (location = 1) in vec2 aTextureCoordinates;

out vec4 Color;
out vec2 textureCoordinates;

uniform mat4 transform;

uniform mat4 modelMatrix;
uniform mat4 view;
uniform mat4 projection;
uniform float scale;

void main()
{
	gl_Position = projection * view * modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	textureCoordinates = vec2(aTextureCoordinates.x, 1.0 - aTextureCoordinates.y); //vec2(time * 0.15, 0);
}
