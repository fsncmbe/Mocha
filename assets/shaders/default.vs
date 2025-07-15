#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform vec4 uColor;

out vec4 color;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    color = uColor;
}