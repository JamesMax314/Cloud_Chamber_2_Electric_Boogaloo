#version 300 es
precision mediump float;

in vec3 fragPos;
out vec4 FragColor;

void main()
{
    float pi = 3.14159;
    float sig = 0.5;
    float luminocity = 1.0 /(sig * sqrt(2.0 * pi)) * exp(-0.5 * pow(fragPos.z/sig, 2.0));

    // FragColor = vec4(luminocity, luminocity, luminocity, luminocity);
    FragColor = vec4(fragPos, luminocity);
}