#version 300 es
precision mediump float;

in vec3 fragPos;
out vec4 FragColor;

uniform samplerBuffer bufferTexture;

uniform vec3 position;
uniform vec3 lightPos;
uniform vec3 lightColour;
uniform float time;

const float PI = 3.14159265359;

float distance_from_sphere(in vec3 p, in vec3 c, float r)
{
	return length(p - c) - r;
}

bool rayIntersectsCube(vec3 rayOrigin, vec3 rayDirection) {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    float scale = 2.0;
    vec3 cubeMin = -1.0*vec3(1.0);
    vec3 cubeMax = vec3(1.0);

    vec3 inverseDirection = 1.0 / rayDirection;

    // Calculate the intersection with X planes
    if (inverseDirection.x >= 0.0) {
        tmin = (cubeMin.x - rayOrigin.x) * inverseDirection.x;
        tmax = (cubeMax.x - rayOrigin.x) * inverseDirection.x;
    } else {
        tmin = (cubeMax.x - rayOrigin.x) * inverseDirection.x;
        tmax = (cubeMin.x - rayOrigin.x) * inverseDirection.x;
    }

    // Calculate the intersection with Y planes
    if (inverseDirection.y >= 0.0) {
        tymin = (cubeMin.y - rayOrigin.y) * inverseDirection.y;
        tymax = (cubeMax.y - rayOrigin.y) * inverseDirection.y;
    } else {
        tymin = (cubeMax.y - rayOrigin.y) * inverseDirection.y;
        tymax = (cubeMin.y - rayOrigin.y) * inverseDirection.y;
    }

    // Check if the ray misses the cube in the XY plane
    if ((tmin > tymax) || (tymin > tmax)) {
        return false;
    }

    // Update tmin and tmax for the XY intersection
    tmin = max(tmin, tymin);
    tmax = min(tmax, tymax);

    // Calculate the intersection with Z planes
    if (inverseDirection.z >= 0.0) {
        tzmin = (cubeMin.z - rayOrigin.z) * inverseDirection.z;
        tzmax = (cubeMax.z - rayOrigin.z) * inverseDirection.z;
    } else {
        tzmin = (cubeMax.z - rayOrigin.z) * inverseDirection.z;
        tzmax = (cubeMin.z - rayOrigin.z) * inverseDirection.z;
    }

    // Check if the ray misses the cube in the XZ plane
    if ((tmin > tzmax) || (tzmin > tmax)) {
        return false;
    }

    // Update tmin and tmax for the final intersection
    tmin = max(tmin, tzmin);
    tmax = min(tmax, tzmax);

    // Check if there is a valid intersection
    return tmax >= 0.0;
}


float sdCube(vec3 p, float s) {
    vec3 d = abs(p) - s * 1.0; // Calculate the distance from the point to the center of the cube
    return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0)); // Return the signed distance
}


float texture(in vec3 pos) 
{
    vec3 centre = vec3(0.0, 0.0, 0.0);
    float rad = 0.5;
    float displacement = sin(8.0 * pos.x) * sin(8.0 * pos.y) * sin(8.0 * pos.z) * 0.25;
    float sphere_0 = distance_from_sphere(pos, vec3(0.0), rad);
    if (sphere_0 + displacement*sin(time*10.0) < 0.0) {
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

    bool eneteredCube = false;
    vec3 col;

    if (rayIntersectsCube(ro, rd)) {
        for (int i = 0; i < maxIterations; i++) {
            float sdf = distance_from_sphere(rayPosition, vec3(0.0, 0.0, 0.0), 0.5);
            float step = 0.0;
            if (sdf > stepSize) {
                step = sdf;
            } else {
                step = stepSize;
            }

            float sdfCube = sdCube(rayPosition, 1.0);
            
            if (!eneteredCube && sdfCube < 0.0) {
                eneteredCube = true;
            } else if (eneteredCube && sdfCube > 0.0) {
                break;
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
    }

    vec3 cloudCol = lightEnergy * lightColour;
    vec3 backGroundCol = vec3(0.0, 0.0, 0.0);
    col = backGroundCol * transmittance + cloudCol;
    return vec4(col, 0.0);
}

void main()
{
    vec2 uv = fragPos.xy * 2.0 - 1.0;

    vec3 camera_position = vec3(0.0, 0.0, -2.0);
    vec3 ro = camera_position + position;
    vec3 rd = fragPos - camera_position;
    float modulus = length(rd);
    rd = rd/modulus;

    vec4 shaded_color = ray_march(ro, rd);

    FragColor = shaded_color;
    // FragColor = vec4(1.0, 0.0, 0.0, 0.0);
}
