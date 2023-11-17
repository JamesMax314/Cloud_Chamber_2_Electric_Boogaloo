#version 300 es
precision mediump float;

in vec4 fragPos;
in vec2 texCoords;
out vec4 FragColor;

uniform mediump sampler2D backgroundTexture; // The framebuffer's color texture
uniform mediump sampler2D cloudTexture;  // The depth texture

vec3 lightColour = vec3(1.0);

float screenWidth = 1080.0;
float screenHeight = 720.0;

const int kernelSize = 9;
const float sigma = 2.0; // Adjust this for the desired blur amount

float gaussian(float x, float sigma) {
    return exp(-(x * x) / (2.0 * sigma * sigma)) / (sqrt(2.0 * 3.14159265358979323846) * sigma);
}

vec4 gaussianClouds() {
    vec2 texelSize = 1.0 / vec2(screenWidth, screenHeight);

    // Gaussian kernel values
    float kernel[kernelSize];
    float totalWeight = 0.0;

    for (int i = 0; i < kernelSize; ++i) {
        float offset = float(i - (kernelSize - 1) / 2);
        kernel[i] = gaussian(offset, sigma);
        totalWeight += kernel[i];
    }

    // Normalize the kernel
    for (int i = 0; i < kernelSize; ++i) {
        kernel[i] /= totalWeight;
    }

    vec4 result = vec4(0.0);

    // Apply the convolution
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            vec2 texP = texCoords + texelSize * vec2(float(i) - (float(kernelSize) - 1.0) / 2.0, float(j) - (float(kernelSize) - 1.0) / 2.0);
            result += texture(cloudTexture, texP) * kernel[i] * kernel[j];
        }
    }
    
    return result;
}

void main() {
    vec4 backTex = texture(backgroundTexture, texCoords);
    vec4 clouds = gaussianClouds();
    // vec4 clouds = texture(cloudTexture, texCoords);
    

    float lightEnergy = clouds.x;
    float transmittance = clouds.y;
    float lampIntensity = clouds.z;

    vec3 cloudCol = lightEnergy * lightColour;
    vec3 col = backTex.xyz*transmittance + cloudCol + lampIntensity*transmittance;

    // backTex = backTex*clouds.w;
    // backTex.xyz = backTex.xyz + clouds.xyz;

    // vec3 outCol = backTex.xyz+clouds;
    FragColor = vec4(col, 0.0);
    // FragColor = vec4(clouds);
}
