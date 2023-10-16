#version 300 es
precision mediump float;

in vec3 fragPos;
out vec4 FragColor;

uniform vec3 position;
uniform vec3 lightPos;
uniform vec3 lightColour;

const float PI = 3.14159265359;

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
        // return snoise(pos*5.0)*2.0;
        return 2.0;
    }

    return 0.0;

    // return snoise(pos);
}

float stepSize = 0.02;

float random(vec2 st) {
    return max(0.0, fract(sin(dot(st, vec2(12.9898, 78.233)) * 43758.5453)))/10.0 + 0.9;
}

float angleBetween(vec3 vectorA, vec3 vectorB) {
    vec3 normalizedA = normalize(vectorA);
    vec3 normalizedB = normalize(vectorB);
    float dotProduct = dot(normalizedA, normalizedB);
    
    // Use acos to find the angle in radians
    float angle = acos(clamp(dotProduct, -1.0, 1.0));

    return angle;
}

float phaseFun(in float theta) {
    float g = 0.3;
    float phase = (1.0-pow(g, 2.0)) / pow(1.0 + pow(g, 2.0) - 2.0*g*cos(theta), 3.0/2.0);
    return phase;
}

float lightMarch(in vec3 rayPosition) {
    vec3 lightDir = lightPos-rayPosition;
    lightDir = lightDir / length(lightDir);

    int numSamples = 30;

    float subDen = 0.0;

    vec3 position = rayPosition;

    for (int j=0; j<numSamples; j++) {
        position += lightDir*stepSize;
        subDen += texture(position)*stepSize*random(rayPosition.xy);
    }

    float transmittance = exp(-subDen);
    return transmittance;
}

vec4 ray_march(in vec3 ro, in vec3 rd)
{
    int maxIterations = 30;
    vec3 rayPosition = ro;

    float transmittance = 1.0;
    float lightEnergy = 0.0;

    for (int i = 0; i < maxIterations; i++) {
        float sdf = distance_from_sphere(rayPosition, vec3(0.0, 0.0, 0.0), 0.5);
        float step = 0.0;
        if (sdf > stepSize) {
            step = sdf;
        } else {
            step = stepSize;
        }

        rayPosition += rd*step*random(rd.xy);

        float density = texture(rayPosition);

        vec3 lightDir = lightPos-rayPosition;
        float theta =  angleBetween(lightDir, rd) ;
        float phase = phaseFun(theta);

        if (density > 0.0) {
            float lightTransmittance = lightMarch(rayPosition);
            
            lightEnergy += density * stepSize * transmittance * lightTransmittance * phase;
            transmittance *= exp(-density * stepSize);
        }

        if (transmittance < 0.01) {
            break;
        }

        
    }

    vec3 cloudCol = lightEnergy * lightColour;
    vec3 backGroundCol = vec3(0.53, 0.81, 0.92);
    vec3 col = backGroundCol * transmittance + cloudCol;
    return vec4(col, 0.0);
}

void main()
{
    vec2 uv = fragPos.xy * 2.0 - 1.0;

    vec3 camera_position = vec3(0.0, 0.0, -1.0);
    vec3 ro = camera_position + position;
    vec3 rd = fragPos - camera_position;
    float modulus = length(rd);
    rd = rd/modulus;

    vec4 shaded_color = ray_march(ro, rd);

    FragColor = shaded_color;
    // FragColor = vec4(1.0, 0.0, 0.0, 0.0);
}
