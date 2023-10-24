#version 300 es
precision mediump float;

in vec4 fragPos;
in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D backgroundTexture; // The framebuffer's color texture
uniform sampler2D cloudTexture;  // The depth texture

vec3 cloudColour = vec3(1.0);

void main() {
    vec4 backTex = texture(backgroundTexture, texCoords);
    vec4 clouds = texture(cloudTexture, texCoords);

    vec3 outCol = backTex.xyz;
    FragColor = vec4(clouds.xyz, 1.0);
}