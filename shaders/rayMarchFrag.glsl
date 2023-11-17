#version 300 es
precision mediump float;

in vec4 fragPos;
in vec2 texCoords;
out vec4 FragColor;

uniform mediump sampler3D texture3D; // Cloud texture
uniform sampler2D framebufferColorTexture; // The framebuffer's color texture
uniform sampler2D framebufferDepthTexture;  // The depth texture

uniform float time;
uniform float texDim;
uniform float nearClip;
uniform float farClip;
uniform float fovRad;

uniform vec3 lightPos;
uniform vec3 minPos;
uniform vec3 maxPos;

uniform mat4 view;

const float PI = 3.14159265359;
const float threshold = 0.0;

float maxTransmissionSamples = 120.0;
float maxLightSamples = 6.0;
float light_fine_step_size = 0.02;
float coarse_step_multiplier = 3.0;
int reset_step_count = 4;


float lightFactor = 10.0;
float fogFactor = 10.0;
float lightCentreZ = 0.0;
float lightStd = 0.2;

vec3 boundingCubeMin = -1.0*vec3(1.0);
vec3 boundingCubeMax = vec3(1.0);

struct Intersection {
    float tNear;
    float tFar;
    bool intersectFound;
    vec3 minIntersect;
    vec3 maxIntersect;
};


float depthToDistance(float depth) {
    float ndcDepth = 2.0 * depth - 1.0;
    float viewSpaceDepth = (2.0 * nearClip * farClip) / (farClip + nearClip - ndcDepth * (farClip - nearClip));
    return viewSpaceDepth;
}

float getDepth(vec3 ro, vec3 rd) {
    vec3 rayLenWorld = rd-ro;
    // vec4 rayLenWorld4 = vec4(rayLenWorld, 0.0);
    vec3 rayLenCam = mat3(view) * rayLenWorld;
    return abs(rayLenCam.z);
}

float getCamDepth(float fovRad) {
    return 1.0/tan(fovRad/2.0);
}

float getTCurrent(vec3 rayOrigin, vec3 rayDirection, vec3 rayPosition) {
    float tCurrent = -999999.0;

    for (int i = 0; i < 3; i++) {
        float t1 = (rayPosition[i] - rayOrigin[i]) / rayDirection[i];
        
        tCurrent = max(tCurrent, t1);
    }

    return tCurrent;
}

// Used to determine where ray enters and exits cloud box
Intersection rayCubeIntersectionPoints(vec3 rayOrigin, vec3 rayDirection, vec3 minVertex, vec3 maxVertex) {
    float tNear = -999999.0;  // A very small negative value
    float tFar = 999999.0;    // A very large positive value
    Intersection intersect;
    intersect.intersectFound = false;
    intersect.tNear = tNear;
    intersect.tFar = tFar;
    intersect.minIntersect = vec3(0.0);
    intersect.maxIntersect = vec3(0.0);

    for (int i = 0; i < 3; i++) {
        float t1 = (minVertex[i] - rayOrigin[i]) / rayDirection[i];
        float t2 = (maxVertex[i] - rayOrigin[i]) / rayDirection[i];
        
        tNear = max(tNear, min(t1, t2));
        tFar = min(tFar, max(t1, t2));
    }

    if (tNear <= tFar && tFar > 0.0) {
        intersect.intersectFound = true;
        intersect.tNear = tNear;
        intersect.tFar = tFar;
        intersect.minIntersect = rayOrigin + tNear * rayDirection;
        intersect.maxIntersect = rayOrigin + tFar * rayDirection;
        return intersect;
    }

    return intersect;  // No intersection; you can return any sentinel value
}

// SDF for track bounding boxes
float sdfCuboid(vec3 p, vec3 minCorner, vec3 maxCorner)
{
    vec3 c = (minCorner + maxCorner) * 0.5;;
    vec3 s = (maxCorner - minCorner);
    float x = max
    (   p.x - c.x - s.x / 2.,
        c.x - p.x - s.x / 2.
    );
    float y = max
    (   p.y - c.y - s.y / 2.,
        c.y - p.y - s.y / 2.
    );
    
    float z = max
    (   p.z - c.z - s.z / 2.,
        c.z - p.z - s.z / 2.
    );
    float d = x;
    d = max(d,y);
    d = max(d,z);
    return d;
}

// Need to move this into a world function
float get_texture(in vec3 pos) 
{
    // return sphereTexture(pos);
    if (sdfCuboid(pos, boundingCubeMin, boundingCubeMax) < 0.0) {
        vec3 stepSize = (maxPos - minPos) / (texDim-1.0); // texture dim - 1 add as uniform
        vec3 texCoords3D = (pos-minPos) / (stepSize*texDim);
        vec4 denvec = texture(texture3D, texCoords3D);
        float den = denvec.r;
        if (den < threshold) {
            den = 0.0;
        }
        return den*0.1;
    }
    return 0.0;
}

// To help avoid banding
float randomStepModifier(vec2 st) {
    return max(0.5, fract(sin(dot(st, vec2(12.9898, 78.233)) * 43758.5453)))/10.0 + 0.9;
}

float angleBetween(vec3 vectorA, vec3 vectorB) {
    vec3 normalizedA = normalize(vectorA);
    vec3 normalizedB = normalize(vectorB);
    float dotProduct = dot(normalizedA, normalizedB);
    
    float angle = acos(dotProduct);

    return angle;
}

// Anisotropic light scattering
float phaseFun(in float theta) {
    float g = 0.3;
    float phase = (1.0-pow(g, 2.0)) / pow(1.0 + pow(g, 2.0) - 2.0*g*cos(theta+PI/2.0), 3.0/2.0);
    return phase;
}

// Simulate light being produced in a strip
float lightBeamFactor(vec3 rayPos) {
    float x = rayPos.z;
    float gaussian = exp(-((x - lightCentreZ) * (x - lightCentreZ)) / (2.0 * lightStd * lightStd));
    return gaussian;
}

// Raycast back to light source
float lightMarch(in vec3 rayPosition) {
    vec3 lightDir = normalize(lightPos-rayPosition);
    vec3 position = rayPosition;

    float fine_step = light_fine_step_size;//(intersect.tFar-tCurrent) / maxLightSamples;
    float step = fine_step;

    float step_size_icrement = (coarse_step_multiplier-1.0)*fine_step/maxLightSamples;

    float subDen = 0.0;
    float sub_den_tmp = 0.0;

    // Increase light step distance each step
    for (int j=0; j<int(maxLightSamples); j++) {
        // Check whether light ray has left the cloud box

        float random_step = randomStepModifier(rayPosition.xy);
        position += lightDir*step*random_step;
        sub_den_tmp = get_texture(position);
        
        subDen += sub_den_tmp*step*random_step;

        step += step_size_icrement;
    }

    float transmittance = exp(-subDen);
    return transmittance;
}

// Ray March/cast to/into fog
vec4 ray_march(in vec3 ro, in vec3 rd)
{
    vec3 rayPosition = ro;

    float transmittance = 1.0;
    float lightEnergy = 0.0;

    bool is_coarse = true;
    int coarse_countdown = 0;

    // Used to draw light location
    float lampIntensity = 0.0;
    float backgroundDepth = depthToDistance(texture(framebufferDepthTexture, texCoords).x);

    // Determine distance to traverse cloud box
    Intersection intersect = rayCubeIntersectionPoints(ro, rd, minPos, maxPos);

    if (intersect.intersectFound) {
        // if outside cloudbox then start ray at edge of cloud box
        float fine_step;
        if (intersect.tNear > 0.0) {
            rayPosition = intersect.minIntersect;
            fine_step = (intersect.tFar-intersect.tNear) / maxTransmissionSamples;
        } else {
            float tCurrent = getTCurrent(ro, rd, rayPosition);
            fine_step = (intersect.tFar-tCurrent) / maxTransmissionSamples;
        }

        float step = fine_step*coarse_step_multiplier;

        for (int i = 0; i < int(maxTransmissionSamples); i++) {
            // Check if ray has left cloud box
            float tCurrent = getTCurrent(ro, rd, rayPosition);
            if (tCurrent > intersect.tFar) {
                break;
            }

            // Update position with random step to avoid banding
            float random_step = randomStepModifier(rayPosition.xy);
            rayPosition += rd*step*random_step;

            // Stop if hits background bubble
            if (getDepth(ro, rayPosition) > backgroundDepth) {
                break;
            }

            float density = get_texture(rayPosition);
            if (density > 0.4 && is_coarse) {
                rayPosition -= rd*step*random_step;
                step = fine_step;
                rayPosition += rd*step*random_step;
                density = get_texture(rayPosition);
                coarse_countdown = reset_step_count;
                is_coarse = false;
            } else if (density < 0.4 && coarse_countdown > 0) {
                coarse_countdown --;
            }

            if (coarse_countdown == 0 && !is_coarse) {
                step = coarse_step_multiplier*fine_step;
                is_coarse = true;
            }

            // Get light scattering factor
            vec3 lightDir = lightPos-rayPosition;
            float theta =  angleBetween(lightDir, rd) ;
            float phase = phaseFun(theta);
            float lightBeamMult = lightBeamFactor(rayPosition);

            // Get light energy and opacity
            if (density > 0.01) {
                float lightTransmittance = lightMarch(rayPosition);
                float cloud_retro_reflectance = 1.0 - exp(-density*step);
                lightEnergy += lightFactor * density * step * transmittance * (lightTransmittance+cloud_retro_reflectance) * phase * lightBeamMult;
                transmittance *= exp(-density * step * fogFactor);
            } else {
                // Draw lamp location on screen
                if (angleBetween(rd, lightPos) < 0.01) {
                    lampIntensity += 1.0*step;
                }
            }

            if (transmittance < 0.01) {
                break;
            }
        }
    }

    return vec4(lightEnergy, transmittance, lampIntensity, 0.0);
}

void main()
{
    mat4 invertView = inverse(view);
    vec4 camera_position = invertView*vec4(0.0, 0.0, 0.0, 1.0);
    vec4 ro = camera_position;
    vec4 mFragPos = fragPos;
    mFragPos.z = -getCamDepth(fovRad);
    vec3 rd = normalize((invertView*mFragPos - camera_position).xyz);
    FragColor = vec4(0.0);

    vec4 shaded_color = ray_march(ro.xyz, rd.xyz);
    FragColor = shaded_color;

    // vec3 p = fragPos.xyz;
    // p.z = time*p.z*10.0;

    // FragColor = abs(rd);

    // float depthCol = texture(framebufferDepthTexture, texCoords).x;
    // FragColor = vec4(vec3(depthToDistance(depthCol)), 1.0);
    // vec4 pixCol = texture(framebufferColorTexture, (fragPos.xy + vec2(1.0))/2.0);
    // FragColor = texture(framebufferColorTexture, texCoords);
    // FragColor = vec4(1.0);
    // FragColor = shaded_color;
    // FragColor = texture(texture3D, p);
    // FragColor = vec4(abs(maxPos), 1.0);
}
