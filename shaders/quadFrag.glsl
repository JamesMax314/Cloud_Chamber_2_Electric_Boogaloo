#version 300 es
precision mediump float;

in vec3 fragPos;
out vec4 FragColor;

void main()
{
    FragColor = vec4(fragPos[0], fragPos[1], 0.2f, 1.0f);
}