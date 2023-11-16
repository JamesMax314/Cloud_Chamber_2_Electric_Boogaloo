#version 300 es
precision mediump float;

in vec4 fragPos;
in vec2 texCoords;
out vec4 FragColor;

uniform mediump sampler2D backgroundTexture; // The framebuffer's color texture
uniform mediump sampler2D cloudTexture;  // The depth texture

vec3 lightColour = vec3(1.0);

float gaussian(float dx, float dy) {
    float coeff = 0.4;
    float rad_squared = dx*dx+dy*dy;
    return (1.0 - rad_squared/2.0)*coeff;
}

float gaussian_proper(float x, float mean, float stddev) {
    return (1.0 / (stddev * sqrt(2.0 * 3.14159265358979323846))) * exp(-((x - mean) * (x - mean)) / (2.0 * stddev * stddev));
}

vec4 gaussianClouds() {
    // float gaussina_kernal[9] = float[9](0.05, 0.11, 0.05,
    //                             0.11, 0.25, 0.11,
    //                             0.05, 0.11, 0.05);
   
    vec4 Color = vec4(0.0);

    int rad_pixels = 5;
    float width = 1080.0;
    float height = 720.0;

    float gauss_mean = 0.0;
    float gauss_std = 10.0/height;
    
    for(int i=-rad_pixels; i<rad_pixels; i++)
    {
		for(int j=-rad_pixels; j<rad_pixels; j++)
        {
            float float_i = float(i)/width;
            float float_j = float(j)/height;
            float x = sqrt(float_i*float_i + float_j*float_j);
			Color += gaussian_proper(x, gauss_mean, gauss_std) * texture(backgroundTexture, texCoords+vec2(i, j));		
        }
    }
    
    return Color;
}

void main() {
    vec4 backTex = texture(backgroundTexture, texCoords);
    // vec4 clouds = gaussianClouds();
    vec4 clouds = texture(cloudTexture, texCoords);
    

    float lightEnergy = clouds.x;
    float transmittance = clouds.y;
    float lampIntensity = clouds.z;

    vec3 cloudCol = lightEnergy * lightColour;
    vec3 col = backTex.xyz*transmittance + cloudCol + lampIntensity*transmittance;

    // backTex = backTex*clouds.w;
    // backTex.xyz = backTex.xyz + clouds.xyz;

    // vec3 outCol = backTex.xyz+clouds;
    FragColor = vec4(col, 1.0);
    // FragColor = vec4(clouds);
}
