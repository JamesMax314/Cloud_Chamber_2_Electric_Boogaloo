#version 300 es
layout(location = 0) in vec3 quad;
// layout(location = 1) in vec3 particlePos;

out vec4 fragPos;
out vec2 texCoords;

// uniform mat4 model;
// uniform mat4 view;
uniform mat4 projection;
uniform float aspect;

vec4 aPos;

void main()
{
    aPos = vec4(quad, 1.0);
    gl_Position = aPos;
    // aPos.x = aPos.x;
    aPos.x = aspect*aPos.x;
    // vec4 aPos4 = view*vec4(aPos, 1.0);

    fragPos = aPos;//particlePos;
    texCoords = (quad.xy + vec2(1.0)) / 2.0;
}
