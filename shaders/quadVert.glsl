#version 300 es
layout(location = 0) in vec3 quad;
layout(location = 1) in vec3 particlePos;

out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 aPos;

void main()
{
    aPos = quad*0.001 + particlePos;
    gl_Position = projection*view*vec4(aPos, 1.0);

    // gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    // fragPos = vec3(abs(view[0][0]), abs(view[1][1]), abs(view[2][2]));//particlePos;
    fragPos = particlePos;//particlePos;
}
