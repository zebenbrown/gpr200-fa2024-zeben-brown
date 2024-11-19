#version 330 core

layout (location = 0) in vec3 aPosition;

uniform mat4 modelMatrix;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 offset;

void main()
{
    gl_Position = projection * view * modelMatrix * vec4(aPosition.x + offset.x, aPosition.y, aPosition.z, 1.0f);

  //  vec2 offset = offsets[gl_InstanceID];
    //gl_Position = vec4(aPosition + offset, 0.0, 1.0);
    //gl_position = vec4(aPosition, 1.0f);
}