#version 300 es
layout(location = 0) in vec3 inPos;

out vec4 fragPos;

void main()
{
    fragPos = vec4(inPos, 1.0);
}
