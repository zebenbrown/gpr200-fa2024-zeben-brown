#version 330 core

out vec4 FragColor;

uniform vec3 torusColor;
void main()
{
    FragColor = vec4(1.0f) * vec4(torusColor, 1.0f);
    //FragColor = vec4(1.0f);
}