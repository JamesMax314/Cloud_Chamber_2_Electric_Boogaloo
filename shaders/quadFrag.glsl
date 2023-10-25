#version 300 es
precision mediump float;

in vec3 fragPos;
out vec4 FragColor;

float lightCentreZ = 0.0;
float lightStd = 0.2;

// Simulate light being produced in a strip
float lightBeamFactor(vec3 rayPos) {
    float x = rayPos.z;
    float gaussian = exp(-((x - lightCentreZ) * (x - lightCentreZ)) / (2.0 * lightStd * lightStd));
    return gaussian;
}

void main()
{
    float luminocity = lightBeamFactor(fragPos);

    FragColor = vec4(luminocity, luminocity, luminocity, 0.0);
    // FragColor = vec4(fragPos, luminocity);
}