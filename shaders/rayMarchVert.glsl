#version 300 es
layout(location = 0) in vec3 quad;
layout(location = 1) in vec3 particlePos;

out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float aspect;

vec3 aPos;

void main()
{
    aPos = quad;
    gl_Position = vec4(aPos, 1.0);
    aPos.x = aPos.x*aspect;
    // vec4 aPos4 = view*vec4(aPos, 1.0);

    fragPos = aPos;//particlePos;
}
