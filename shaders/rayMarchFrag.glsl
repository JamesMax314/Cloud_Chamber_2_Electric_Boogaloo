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


vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
     return mod289(((x*34.0)+10.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
  { 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //   x0 = x0 - 0.0 + 0.0 * C.xxx;
  //   x1 = x0 - i1  + 1.0 * C.xxx;
  //   x2 = x0 - i2  + 2.0 * C.xxx;
  //   x3 = x0 - 1.0 + 3.0 * C.xxx;
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

// Permutations
  i = mod289(i); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients: 7x7 points over a square, mapped onto an octahedron.
// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.5 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 105.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
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

    int numSamples = 100;

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
    int maxIterations = 100;
    vec3 rayPosition = ro;

    float transmittance = 1.0;
    float lightEnergy = 0.0;

    for (int i = 0; i < maxIterations; i++) {
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

        rayPosition += rd*stepSize*random(rd.xy);
        
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