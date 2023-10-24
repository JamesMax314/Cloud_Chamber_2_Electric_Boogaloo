#version 300 es
layout(location = 0) in vec3 inPos;

out vec4 fragPos;
out vec2 texCoords;

vec4 aPos;

void main()
{
    aPos = vec4(inPos, 1.0);
    gl_Position = aPos;
    fragPos = aPos;

    texCoords = (inPos.xy + vec2(1.0)) / 2.0;
}