#version 300 es
precision mediump float;

in vec3 fragPos;
out vec4 FragColor;

uniform vec3 position;

float distance_from_sphere(in vec3 p, in vec3 c, float r)
{
	return length(p - c) - r;
}

float texture(in vec3 pos) 
{
    vec3 centre = vec3(0.0, 0.0, 0.0);
    float rad = 0.5;
    float cenRad = 0.3;
    float dist = length(pos - centre);
    if (dist < rad && dist > cenRad) {
        return 0.01;
    }

    return 0.0;
}

vec4 ray_march(in vec3 ro, in vec3 rd)
{
    int maxIterations = 1000;
    float totalDensity = 0.0;
    float stepSize = 0.01;
    vec3 rayPosition = ro;
    for (int i = 0; i < maxIterations; i++) {
        // Sample the cloud density at the current position
        float cloudDensity = texture(rayPosition);

        // Accumulate cloud density
        totalDensity += cloudDensity;

        // Move the ray along its direction
        rayPosition += stepSize * rd;
    }
    return vec4(totalDensity, totalDensity, totalDensity, 0.0);
}

void main()
{
    vec2 uv = fragPos.xy * 2.0 - 1.0;

    vec3 camera_position = vec3(0.0, 0.0, -5.0);
    vec3 ro = camera_position + position;
    vec3 rd = fragPos - camera_position;
    float modulus = length(rd);
    rd = rd/modulus;

    vec4 shaded_color = ray_march(ro, rd);

    FragColor = shaded_color;
    // FragColor = vec4(1.0, 0.0, 0.0, 0.0);
}