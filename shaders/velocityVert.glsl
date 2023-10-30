#version 300 es
layout(location = 0) in vec3 inPos;

out vec2 texturePos;

void main()
{
    gl_Position = vec4(inPos, 1.0);
    texturePos = (inPos.xy + vec2(1.0)) / 2.0;
}
