#version 300 es
layout(location = 0) in vec3 quad;
layout(location = 1) in vec3 particlePos;

out vec3 fragPos;

vec3 aPos;

void main()
{
    aPos = quad*0.1+particlePos*0.1;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    fragPos = particlePos;
}