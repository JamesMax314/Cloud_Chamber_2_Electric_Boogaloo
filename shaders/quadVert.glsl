#version 300 es
layout(location = 0) in vec3 quad;
layout(location = 1) in vec3 particlePos;

out vec3 fragPos;

vec3 aPos;

void main()
{
    aPos = quad + particlePos;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    fragPos = vec3(1.0,1.0,1.0);//particlePos;
}
