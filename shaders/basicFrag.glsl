#version 400 core
precision mediump float;

in vec4 fragPos;
out vec4 FragColor;

void main()
{
    FragColor = vec4(abs(fragPos.xyz), 1.0);
}