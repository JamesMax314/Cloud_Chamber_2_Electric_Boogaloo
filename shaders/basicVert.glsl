#version 300 es
layout(location = 0) in vec3 inPos;

out vec4 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec4 aPos;

void main()
{
    aPos = projection*view*vec4(inPos, 1.0);
    gl_Position = aPos;
    fragPos = vec4(inPos, 1.0);
}