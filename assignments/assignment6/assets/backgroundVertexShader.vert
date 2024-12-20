#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoordinates;

out vec3 FragPosition;
out vec3 Normal;
out vec2 textureCoordinates;

uniform mat4 modelMatrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPosition = vec3(modelMatrix * vec4(aPos, 1.0f));
	Normal = mat3(transpose(inverse(modelMatrix))) * aNormal;
	textureCoordinates = aTextureCoordinates;

	gl_Position = projection * view * modelMatrix * vec4(FragPosition, 1.0f);
}
