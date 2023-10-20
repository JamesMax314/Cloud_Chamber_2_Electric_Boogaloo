#version 300 es

in vec3 Position;
out vec3 vPosition;
uniform float time;
uniform int is_track_vert;

vec4 permute(vec4 x) {
     vec4 xm = mod(x, 289.0);
     return mod(((xm*34.0)+10.0)*xm, 289.0);
}

float psrdnoise(vec3 x, vec3 period, float alpha, out vec3 gradient)
{

#ifndef PERLINGRID
  // Transformation matrices for the axis-aligned simplex grid
  const mat3 M = mat3(0.0, 1.0, 1.0,
                      1.0, 0.0, 1.0,
                      1.0, 1.0, 0.0);

  const mat3 Mi = mat3(-0.5, 0.5, 0.5,
                        0.5,-0.5, 0.5,
                        0.5, 0.5,-0.5);
#endif

  vec3 uvw;

  // Transform to simplex space (tetrahedral grid)
#ifndef PERLINGRID
  // Use matrix multiplication, let the compiler optimise
  uvw = M * x;
#else
  // Optimised transformation to uvw (slightly faster than
  // the equivalent matrix multiplication on most platforms)
  uvw = x + dot(x, vec3(1.0/3.0));
#endif

  // Determine which simplex we're in, i0 is the "base corner"
  vec3 i0 = floor(uvw);
  vec3 f0 = fract(uvw); // coords within "skewed cube"

  // To determine which simplex corners are closest, rank order the
  // magnitudes of u,v,w, resolving ties in priority order u,v,w,
  // and traverse the four corners from largest to smallest magnitude.
  // o1, o2 are offsets in simplex space to the 2nd and 3rd corners.
  vec3 g_ = step(f0.xyx, f0.yzz); // Makes comparison "less-than"
  vec3 l_ = 1.0 - g_;             // complement is "greater-or-equal"
  vec3 g = vec3(l_.z, g_.xy);
  vec3 l = vec3(l_.xy, g_.z);
  vec3 o1 = min( g, l );
  vec3 o2 = max( g, l );

  // Enumerate the remaining simplex corners
  vec3 i1 = i0 + o1;
  vec3 i2 = i0 + o2;
  vec3 i3 = i0 + vec3(1.0);

  vec3 v0, v1, v2, v3;

  // Transform the corners back to texture space
#ifndef PERLINGRID
  v0 = Mi * i0;
  v1 = Mi * i1;
  v2 = Mi * i2;
  v3 = Mi * i3;
#else
  // Optimised transformation (mostly slightly faster than a matrix)
  v0 = i0 - dot(i0, vec3(1.0/6.0));
  v1 = i1 - dot(i1, vec3(1.0/6.0));
  v2 = i2 - dot(i2, vec3(1.0/6.0));
  v3 = i3 - dot(i3, vec3(1.0/6.0));
#endif

  // Compute vectors to each of the simplex corners
  vec3 x0 = x - v0;
  vec3 x1 = x - v1;
  vec3 x2 = x - v2;
  vec3 x3 = x - v3;

  if(any(greaterThan(period, vec3(0.0)))) {
    // Wrap to periods and transform back to simplex space
    vec4 vx = vec4(v0.x, v1.x, v2.x, v3.x);
    vec4 vy = vec4(v0.y, v1.y, v2.y, v3.y);
    vec4 vz = vec4(v0.z, v1.z, v2.z, v3.z);
	// Wrap to periods where specified
	if(period.x > 0.0) vx = mod(vx, period.x);
	if(period.y > 0.0) vy = mod(vy, period.y);
	if(period.z > 0.0) vz = mod(vz, period.z);
    // Transform back
#ifndef PERLINGRID
    i0 = M * vec3(vx.x, vy.x, vz.x);
    i1 = M * vec3(vx.y, vy.y, vz.y);
    i2 = M * vec3(vx.z, vy.z, vz.z);
    i3 = M * vec3(vx.w, vy.w, vz.w);
#else
    v0 = vec3(vx.x, vy.x, vz.x);
    v1 = vec3(vx.y, vy.y, vz.y);
    v2 = vec3(vx.z, vy.z, vz.z);
    v3 = vec3(vx.w, vy.w, vz.w);
    // Transform wrapped coordinates back to uvw
    i0 = v0 + dot(v0, vec3(1.0/3.0));
    i1 = v1 + dot(v1, vec3(1.0/3.0));
    i2 = v2 + dot(v2, vec3(1.0/3.0));
    i3 = v3 + dot(v3, vec3(1.0/3.0));
#endif
	// Fix rounding errors
    i0 = floor(i0 + 0.5);
    i1 = floor(i1 + 0.5);
    i2 = floor(i2 + 0.5);
    i3 = floor(i3 + 0.5);
  }

  // Compute one pseudo-random hash value for each corner
  vec4 hash = permute( permute( permute( 
              vec4(i0.z, i1.z, i2.z, i3.z ))
            + vec4(i0.y, i1.y, i2.y, i3.y ))
            + vec4(i0.x, i1.x, i2.x, i3.x ));

  // Compute generating gradients from a Fibonacci spiral on the unit sphere
  vec4 theta = hash * 3.883222077;  // 2*pi/golden ratio
  vec4 sz    = hash * -0.006920415 + 0.996539792; // 1-(hash+0.5)*2/289
  vec4 psi   = hash * 0.108705628 ; // 10*pi/289, chosen to avoid correlation

  vec4 Ct = cos(theta);
  vec4 St = sin(theta);
  vec4 sz_prime = sqrt( 1.0 - sz*sz ); // s is a point on a unit fib-sphere

  vec4 gx, gy, gz;

  // Rotate gradients by angle alpha around a pseudo-random ortogonal axis
#ifdef FASTROTATION
  // Fast algorithm, but without dynamic shortcut for alpha = 0
  vec4 qx = St;         // q' = norm ( cross(s, n) )  on the equator
  vec4 qy = -Ct; 
  vec4 qz = vec4(0.0);

  vec4 px =  sz * qy;   // p' = cross(q, s)
  vec4 py = -sz * qx;
  vec4 pz = sz_prime;

  psi += alpha;         // psi and alpha in the same plane
  vec4 Sa = sin(psi);
  vec4 Ca = cos(psi);

  gx = Ca * px + Sa * qx;
  gy = Ca * py + Sa * qy;
  gz = Ca * pz + Sa * qz;
#else
  // Slightly slower algorithm, but with g = s for alpha = 0, and a
  // useful conditional speedup for alpha = 0 across all fragments
  if(alpha != 0.0) {
    vec4 Sp = sin(psi);          // q' from psi on equator
    vec4 Cp = cos(psi);

    vec4 px = Ct * sz_prime;     // px = sx
    vec4 py = St * sz_prime;     // py = sy
    vec4 pz = sz;

    vec4 Ctp = St*Sp - Ct*Cp;    // q = (rotate( cross(s,n), dot(s,n))(q')
    vec4 qx = mix( Ctp*St, Sp, sz);
    vec4 qy = mix(-Ctp*Ct, Cp, sz);
    vec4 qz = -(py*Cp + px*Sp);

    vec4 Sa = vec4(sin(alpha));       // psi and alpha in different planes
    vec4 Ca = vec4(cos(alpha));

    gx = Ca * px + Sa * qx;
    gy = Ca * py + Sa * qy;
    gz = Ca * pz + Sa * qz;
  }
  else {
    gx = Ct * sz_prime;  // alpha = 0, use s directly as gradient
    gy = St * sz_prime;
    gz = sz;  
  }
#endif

  // Reorganize for dot products below
  vec3 g0 = vec3(gx.x, gy.x, gz.x);
  vec3 g1 = vec3(gx.y, gy.y, gz.y);
  vec3 g2 = vec3(gx.z, gy.z, gz.z);
  vec3 g3 = vec3(gx.w, gy.w, gz.w);

  // Radial decay with distance from each simplex corner
  vec4 w = 0.5 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3));
  w = max(w, 0.0);
  vec4 w2 = w * w;
  vec4 w3 = w2 * w;

  // The value of the linear ramp from each of the corners
  vec4 gdotx = vec4(dot(g0,x0), dot(g1,x1), dot(g2,x2), dot(g3,x3));

  // Multiply by the radial decay and sum up the noise value
  float n = dot(w3, gdotx);

  // Compute the first order partial derivatives
  vec4 dw = -6.0 * w2 * gdotx;
  vec3 dn0 = w3.x * g0 + dw.x * x0;
  vec3 dn1 = w3.y * g1 + dw.y * x1;
  vec3 dn2 = w3.z * g2 + dw.z * x2;
  vec3 dn3 = w3.w * g3 + dw.w * x3;
  gradient = 39.5 * (dn0 + dn1 + dn2 + dn3);

  // Scale the return value to fit nicely into the range [-1,1]
  return 39.5 * n;
}

vec3 p1 = vec3(0.0, 0.0, 0.0);
vec3 p2 = vec3(0.0, 0.0, 0.0);
vec3 p3 = vec3(0.0, 0.0, 0.0);    

float fbm(vec3 pos, float alpha, out vec3 grad)
{
    int N = 8;

    vec3 p = vec3(0.0);
    float w = 1.0; //Weight of noise
    float s = 1.0; //Scale of noise
    float n = 0.0; //Output noise value

    vec3 g;
    vec3 gsum = vec3(0.0);

    for(int i = 0; i<N ; i++){
        n += w * psrdnoise(s*pos + 0.13*gsum, p, s*alpha, g);
        gsum += w*g;
        s *= 2.0;
        w *= 0.5; //pow(s,-5.0/3.0);
    }

    grad = gsum;
    return n;

}

vec3 offset2 = vec3(1000.0, 0.0, 0.0);
vec3 offset3 = vec3(0.0, 0.0, 1000.0);

vec3 curlnoise(vec3 pos, float alpha)
{


    vec3 grad1;
    vec3 grad2;
    vec3 grad3;
    float n1 = fbm(pos, alpha, grad1);
    float n2 = fbm(pos+offset2, alpha, grad2);
    float n3 = fbm(pos+offset3, alpha, grad3);

    float w = sqrt(n1*n1 + n2*n2 + n3*n3);

    vec3 velocity = w*vec3((grad3.y-grad2.z),(grad1.z-grad3.x),(grad2.x-grad1.y));

    return velocity;

}

float random(vec2 st)
{
    return 2.0*fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123)-1.0;
} 

void main()
{
    float alpha = 0.1*time;
    vec3 velocity = curlnoise(Position, alpha);
    //velocity = normalise(velocity);
    vec3 drift = vec3(0.0, 0.0, -0.0005);
    vPosition = Position + 0.00003*velocity + drift;
    
    if(is_track_vert==0){
        if(abs(vPosition.x)>1.0 || abs(vPosition.y)>1.0 || abs(vPosition.z)>1.0){
    	    float randx = random(vec2(drift.y, vPosition.z));
    	    float randy = random(vec2(drift.z, vPosition.x));
    	    vPosition = vec3(randx, randy, 1.0);
    	}
    }

    gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
    gl_PointSize = 10.0;
}
