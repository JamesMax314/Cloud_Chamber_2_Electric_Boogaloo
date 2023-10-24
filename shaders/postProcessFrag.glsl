#version 300 es
precision mediump float;

in vec4 fragPos;
in vec2 texCoords;
out vec4 FragColor;

uniform mediump sampler2D backgroundTexture; // The framebuffer's color texture
uniform mediump sampler2D cloudTexture;  // The depth texture

vec3 cloudColour = vec3(1.0);

void main() {
    vec4 backTex = texture(backgroundTexture, texCoords);
    vec4 clouds = texture(cloudTexture, texCoords);

    backTex = backTex*clouds.w;
    backTex.xyz = backTex.xyz + clouds.xyz;

    // vec3 outCol = backTex.xyz+clouds;
    FragColor = backTex;
}