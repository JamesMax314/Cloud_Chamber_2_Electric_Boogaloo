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
uniform vec3 lightColour;
uniform vec3 minPos;
uniform vec3 maxPos;

uniform mat4 view;

const float PI = 3.14159265359;
const float threshold = 0.0;

int maxIterations = 16;
int maxLightSamples = 16;

float stepSize = 0.025;
float lightFactor = 5.0;

vec3 boundingCubeMin = -1.0*vec3(1.0);
vec3 boundingCubeMax = vec3(1.0);


float depthToDistance(float depth) {
    float ndcDepth = 2.0 * depth - 1.0;
    float viewSpaceDepth = (2.0 * nearClip * farClip) / (farClip + nearClip - ndcDepth * (farClip - nearClip));
    return viewSpaceDepth;
}

float getCamDepth(float fovRad) {
    return 1.0/tan(fovRad/2.0);
}

// Used to determine whether a ray should be calculated
bool rayIntersectsCube(vec3 rayOrigin, vec3 rayDirection, vec3 cubeMin, vec3 cubeMax) {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    float scale = 2.0;


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

float distance_from_sphere(in vec3 p, in vec3 c, float r)
{
	return length(p - c) - r;
}

float sphereTexture(in vec3 pos) 
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

// Need to move this into a world function
float texture(in vec3 pos) 
{
    // return sphereTexture(pos);
    if (sdfCuboid(pos, boundingCubeMin, boundingCubeMax) < 0.0) {
        if (sdfCuboid(pos, minPos, maxPos) < 0.0) {
            vec3 stepSize = (maxPos - minPos) / (texDim-1.0); // texture dim - 1 add as uniform
            vec3 texCoords = (pos-minPos) / (stepSize*texDim);
            float den = texture(texture3D, texCoords).r;
            if (den < threshold) {
                den = 0.0;
            }
            return den*0.1;
        }
        return 0.0;
    }
    return 0.0;
}

// To help avoid banding
float randomStepModifier(vec2 st) {
    return max(0.0, fract(sin(dot(st, vec2(12.9898, 78.233)) * 43758.5453)))/10.0 + 0.9;
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

// Raycast back to light source
float lightMarch(in vec3 rayPosition) {
    vec3 lightDir = normalize(lightPos-rayPosition);

    float subDen = 0.0;

    vec3 position = rayPosition;

    for (int j=0; j<maxLightSamples; j++) {
        position += lightDir*stepSize*randomStepModifier(rayPosition.xy);
        subDen += texture(position)*stepSize*randomStepModifier(rayPosition.xy);
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

    // Used to draw light location
    float lampIntensity = 0.0;

    bool eneteredCube = false;
    vec3 col;

    vec3 backGroundCol = vec3(0.0, 0.0, 0.0);

    if (rayIntersectsCube(ro, rd, boundingCubeMin, boundingCubeMax)) {
        for (int i = 0; i < maxIterations; i++) {
            // float sdf = distance_from_sphere(rayPosition, vec3(0.0, 0.0, 0.0), 0.5);

            // Ray march to edge of bounding cuboid
            float sdf = sdfCuboid(rayPosition, minPos, maxPos);
            float step = 0.0;
            if (sdf > stepSize) {
                step = sdf;
            } else {
                step = stepSize;
            }

            // Check if ray has left cloud chamber
            float sdfCube = sdfCuboid(rayPosition, boundingCubeMin, boundingCubeMax);
            
            if (!eneteredCube && sdfCube < 0.0) {
                eneteredCube = true;
            } else if (eneteredCube && sdfCube > 0.0) {
                break;
            }

            // Update position with random step to avoid banding
            rayPosition += rd*step*randomStepModifier(rd.xy);

            float density = texture(rayPosition);

            // Get light scattering factor
            vec3 lightDir = lightPos-rayPosition;
            float theta =  angleBetween(lightDir, rd) ;
            float phase = phaseFun(theta);

            // Get light energy and opacity
            if (density > 0.0) {
                float lightTransmittance = lightMarch(rayPosition);
                
                lightEnergy += lightFactor * density * stepSize * transmittance * lightTransmittance * phase;
                transmittance *= exp(-density * stepSize);
            } else {
                if (angleBetween(rd, lightPos) < 0.01) {
                    lampIntensity += 1.0*stepSize;
                }
            }

            if (transmittance < 0.01) {
                break;
            }
        }
        vec4 pixCol = texture(framebufferColorTexture, texCoords);
        backGroundCol = pixCol.xyz;
    } else {
        backGroundCol = vec3(0.6, 0.2, 0.4);
    }

    vec3 cloudCol = lightEnergy * lightColour;
    col = backGroundCol * transmittance + cloudCol + lampIntensity*transmittance;
    return vec4(col, 0.0);
}

void main()
{
    vec4 camera_position = view*vec4(0.0, 0.0, 0.0, 1.0);
    vec4 ro = camera_position;
    vec4 mFragPos = fragPos;
    mFragPos.z = -getCamDepth(fovRad);
    vec3 rd = normalize((view*mFragPos - camera_position).xyz);
    float modulus = length(rd);
    rd = rd/modulus;

    vec4 shaded_color = ray_march(ro.xyz, rd.xyz);
    vec3 p = fragPos.xyz;
    p.z = time*p.z*10.0;

    // FragColor = abs(rd);

    // float depthCol = texture(framebufferDepthTexture, (fragPos.xy + vec2(1.0))/2.0).x;
    // vec4 pixCol = texture(framebufferColorTexture, (fragPos.xy + vec2(1.0))/2.0);
    // FragColor = texture(framebufferColorTexture, texCoords);
    // FragColor = vec4(1.0);
    FragColor = shaded_color;
    // FragColor = texture(texture3D, p);
    // FragColor = vec4(abs(maxPos), 1.0);
}
